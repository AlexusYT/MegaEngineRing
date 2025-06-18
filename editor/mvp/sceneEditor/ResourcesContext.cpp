//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 03.09.24.
//

#include "ResourcesContext.h"

#include <future>

#include "KwasarEngine/context/IApplication.h"
#include "KwasarEngine/resources/IResource.h"
#include "KwasarEngine/resources/IResourceBundle.h"
#include "KwasarEngine/resources/LoadedResources.h"
#include "KwasarEngine/resources/ResourceLoadResult.h"
#include "KwasarEngine/resources/ResourceLoader.h"
#include "KwasarEngine/resources/ResourceLoaders.h"
#include "KwasarEngine/utils/UUID.h"

namespace ked {
std::shared_ptr<ke::ResourceLoadResult> ResourcesContext::loadResourceSync(const std::string &pResourceUri) {
	std::promise<const std::shared_ptr<ke::ResourceLoadResult>> promise;

	loadResourceAsync(pResourceUri, [&promise](const std::shared_ptr<ke::ResourceLoadResult> &pResult) {
		if (!pResult->isPreLoaded()) promise.set_value(pResult);
	});
	auto future = promise.get_future();
	future.wait();
	return future.get();
}

void ResourcesContext::preloadResources() {
	std::vector<std::string> uris;
	application->getResourceBundle()->listResources(uris);
	std::lock_guard lock(queueMutex);
	for (auto uri: uris) {
		auto iter = queue.find(uri);
		if (iter == queue.end()) { iter = queue.emplace(uri, std::make_shared<Request>()).first; }
		auto &request = iter->second;
		request->uri = uri;
		request->type = RequestType::PRELOAD;
		request->callbackSignal.connect([](const std::shared_ptr<ke::ResourceLoadResult> &pResult) {
			if (pResult->isErrored()) {
				ke::Logger::error(pResult->getError());
				return;
			}
			/*ke::Logger::out("Resource {} ({}) preloaded", pResult->getRequestedUri(),
									pResult->getResource()->getUuid());*/
		});
	}
	cv.notify_all();
}

void ResourcesContext::loadResourceAsync(
	const std::string &pResourceUri,
	const sigc::slot<void(const std::shared_ptr<ke::ResourceLoadResult> &pResult)> &pSlot) {
	if (std::shared_ptr<ke::IResource> foundResource = resources->getResource(pResourceUri)) {
		auto result = ke::ResourceLoadResult::create();
		result->setResource(foundResource);
		result->setRequestedUri(pResourceUri);

		if (!foundResource->isIncomplete()) {
			result->setState(ke::ResourceLoadResult::State::READY);
			callSlot(result, pSlot);
			return;
		}
	}
	{
		std::lock_guard lock(processingQueueMutex);
		auto iter = processingQueue.find(pResourceUri);
		if (iter != processingQueue.end()) {
			iter->second->callbackSignal.connect(pSlot);
			return;
		}
	}
	std::lock_guard lock(queueMutex);
	auto iter = queue.find(pResourceUri);
	if (iter == queue.end()) {
		iter = queue.emplace(pResourceUri, std::make_shared<Request>()).first;
		auto &request = iter->second;
		request->uri = pResourceUri;
		request->type = RequestType::LOAD;
	}
	iter->second->callbackSignal.connect(pSlot);
	cv.notify_all();
}

void ResourcesContext::loop(const std::stop_token &pToken) {
	while (!pToken.stop_requested()) {
		{
			std::unique_lock lck(waitMutex);
			//ke::Logger::out("Thread {} waiting", std::this_thread::get_id());
			cv.wait(lck, [this, pToken]() { return !queue.empty() || pToken.stop_requested(); });
			//ke::Logger::out("Thread {} notified", std::this_thread::get_id());
		}

		std::shared_ptr<Request> request;
		{
			std::lock_guard lock1(queueMutex);
			if (queue.empty()) continue;
			auto iter = queue.begin();
			request = iter->second;
			std::lock_guard lock(processingQueueMutex);
			queue.erase(iter);
			processingQueue.emplace(request->uri, request);
		}
		//ke::Logger::out("Thread {} grabbed request {}", std::this_thread::get_id(), request->uri.string());
		processRequest(request);
		{
			std::lock_guard lock(processingQueueMutex);
			processingQueue.erase(request->uri);
		}
	}
}

void ResourcesContext::processRequest(const std::shared_ptr<Request> &pRequest) {
	auto result = ke::ResourceLoadResult::create();
	//PRELOAD incomplete = PRELOADED
	//PRELOAD complete = PRELOADED
	//LOAD incomplete = resource loading required
	//LOAD complete = READY
	std::shared_ptr<ke::IResource> resource;
	if (std::shared_ptr<ke::IResource> foundResource = resources->getResource(pRequest->uri.string())) {
		result->setResource(foundResource);
		result->setRequestedUri(pRequest->uri.string());

		if (pRequest->type == RequestType::PRELOAD) {
			result->setState(ke::ResourceLoadResult::State::PRELOADED);
			callSlot(result, pRequest->callbackSignal);
			return;
		}
		if (!foundResource->isIncomplete()) {
			result->setState(ke::ResourceLoadResult::State::READY);
			callSlot(result, pRequest->callbackSignal);
			return;
		}
		resource = foundResource;
	}


	if (!pRequest->uri.has_extension()) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Unable to load resource");
		msg->setMessage("No resource extension in uri");
		msg->addInfoLine("Resource URI: {}", pRequest->uri.string());
		result->setError(msg);
		result->setRequestedUri(pRequest->uri.string());
		callSlot(result, pRequest->callbackSignal);
		return;
	}

	std::shared_ptr<ke::IResourceLoader> loader = getLoader(pRequest);
	if (!loader) return;
	std::shared_ptr<std::istream> stream = getResourceStream(pRequest);
	if (!stream) return;
	if (!resource) resource = loader->createResource();

	auto startTime = std::chrono::steady_clock::now();
	if (pRequest->type == RequestType::PRELOAD) {
		if (auto msg = loader->preload(this, stream, resource)) {
			msg->setTitle("Unable to preload resource");
			result->setError(msg);
			result->setRequestedUri(pRequest->uri.string());
			callSlot(result, pRequest->callbackSignal);
			return;
		}
		{
			std::lock_guard lock(headersLengthMutex);
			headersLength.emplace(resource, stream->tellg());
		}
		resources->addResource(pRequest->uri.string(), resource);
	} else {
		{
			std::lock_guard lock(headersLengthMutex);
			stream->seekg(headersLength.at(resource));
			headersLength.erase(resource);
		}
		if (auto msg = loader->load(this, stream, resource)) {
			msg->setTitle("Unable to load resource");
			result->setError(msg);
			result->setRequestedUri(pRequest->uri.string());
			callSlot(result, pRequest->callbackSignal);
			return;
		}
		resources->markResourceComplete(pRequest->uri.string());
	}
	try {
		auto endTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		if (duration.count() > 500) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Too long duration");
			msg->setMessage("Resource loading took to long");
			msg->addInfoLine("Resource URI: {}", pRequest->uri.string());
			msg->addInfoLine("Duration: {}ms", duration.count());
			ke::Logger::warn(msg->getReport(false));
		}
		result->setResource(resource);
		result->setRequestedUri(pRequest->uri.string());

		result->setState(ke::ResourceLoadResult::State::READY);
		callSlot(result, pRequest->callbackSignal);
	}
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Unable to load resource");
		msg->setMessage("Exception thrown while executing request");
		msg->addInfoLine("Resource URI: {}", pRequest->uri.string());
		result->setError(msg);
		result->setRequestedUri(pRequest->uri.string());
		callSlot(result, pRequest->callbackSignal);
	}
}

void ResourcesContext::callSlot(
	const std::shared_ptr<ke::ResourceLoadResult> &pResult,
	const sigc::slot<void(const std::shared_ptr<ke::ResourceLoadResult> &pResult)> &pSlot) {
	try { pSlot(pResult); }
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to send loading result to the callback");
		msg->setMessage("Exception thrown in callback");
		msg->addInfoLine("Result state: {}", pResult->getStateStr());
		msg->addInfoLine("Requested resource URI: {}", pResult->getRequestedUri());
		if (pResult->isErrored()) {
			msg->addInfoLine("Result error reported earlier");
			ke::Logger::error(pResult->getError());
		}
		ke::Logger::error(msg);
	}
}

std::shared_ptr<ke::IResourceLoader> ResourcesContext::getLoader(const std::shared_ptr<Request> &pRequest) {
	try {
		auto loader = ke::ResourceLoaders::getInstance()->getLoader(pRequest->uri.extension());
		if (!loader) {
			auto msg = ke::ReportMessage::create();
			msg->setTitle("Unable to load resource");
			msg->setMessage("No loader registered that can load such resource");
			msg->addInfoLine("Resource URI: {}", pRequest->uri.string());
			auto result = ke::ResourceLoadResult::create();
			result->setError(msg);
			result->setRequestedUri(pRequest->uri.string());
			callSlot(result, pRequest->callbackSignal);
			return nullptr;
		}
		return loader;
	}
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Unable to load resource");
		msg->setMessage("Exception occurred while getting the resource loader");
		msg->addInfoLine("Resource URI: {}", pRequest->uri.string());
		auto result = ke::ResourceLoadResult::create();
		result->setError(msg);
		result->setRequestedUri(pRequest->uri.string());
		callSlot(result, pRequest->callbackSignal);
		return nullptr;
	}
}

std::shared_ptr<std::istream> ResourcesContext::getResourceStream(const std::shared_ptr<Request> &pRequest) const {
	try {
		std::shared_ptr<std::istream> stream;
		if (auto msg = application->getResourceBundle()->getResourceStream(pRequest->uri.string(), stream)) {
			msg->setTitle("Unable to load resource");
			auto result = ke::ResourceLoadResult::create();
			result->setError(msg);
			result->setRequestedUri(pRequest->uri.string());
			callSlot(result, pRequest->callbackSignal);
			return nullptr;
		}
		return stream;
	}
	catch (...) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Unable to load resource");
		msg->setMessage("Exception occurred while getting the resource stream");
		auto result = ke::ResourceLoadResult::create();
		result->setError(msg);
		result->setRequestedUri(pRequest->uri.string());
		callSlot(result, pRequest->callbackSignal);
		return nullptr;
	}
}
} // namespace ked
