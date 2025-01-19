//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 02.02.24.
//

#include <condition_variable>
#include <future>

#include "EngineSDK/main/IApplication.h"
#include "EngineSDK/main/resources/IResourceBundle.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/ResourceLoader.h"
#include "EngineSDK/main/resources/ResourceLoaders.h"
#include "EngineUtils/utils/Logger.h"
#include "EngineUtils/utils/ReportMessage.h"
#ifndef EDITOR_SDK
	#include "EngineSDK/main/resources/LoadedResources.h"
	#include "ResourcesWindow.h"

namespace mer::sdk::main {
std::condition_variable cv;

ResourcesWindow::ResourcesWindow()
	: resources(LoadedResources::create()),
	  thread([this](const std::stop_token &pToken) { this->resourceLoop(pToken); }) {
	thread.detach();
}

std::shared_ptr<ResourceLoadResult> ResourcesWindow::loadResourceSync(const std::string &pResourceUri) {

	std::lock_guard lock(queueMutex);
	std::promise<const std::shared_ptr<ResourceLoadResult>> promise;

	queue.emplace_back(pResourceUri,
					   [&promise](const std::shared_ptr<ResourceLoadResult> &pResult) { promise.set_value(pResult); });
	cv.notify_one();
	return promise.get_future().get();
}

void ResourcesWindow::loadResourceAsync(const std::string &pResourceUri, const LoadingFinishedSlot &pSlot) {
	{
		std::lock_guard lock(queueMutex);
		queue.emplace_back(pResourceUri, pSlot);
	}
	cv.notify_one();
}

void ResourcesWindow::requestStopThread() { thread.request_stop(); }

void ResourcesWindow::resourceLoop(const std::stop_token &pToken) {
	while (!pToken.stop_requested()) {
		std::unique_lock lck(waitMutex);
		cv.wait(lck, [this, pToken] { return !queue.empty() || pToken.stop_requested(); });
		getContext()->makeCurrent();
		for (auto &[resourceUri, slot]: queue) {

			auto result = ResourceLoadResult::create();
			if (std::shared_ptr<IResource> resource = resources->getResource(resourceUri)) {
				result->setResource(resource);
				result->setState(ResourceLoadResult::State::READY);
				result->setRequestedUri(resourceUri);
				callSlot(result, slot);
				continue;
			}

			std::filesystem::path uri;
			try {

				if (!uri.has_extension()) {
					auto msg = sdk::utils::ReportMessage::create();
					msg->setTitle("Unable to load resource");
					msg->setMessage("No resource extension in uri");
					msg->addInfoLine("Resource URI: {}", uri.string());
					msg->addInfoLine("Requested resource URI: {}", resourceUri);
					result->setError(msg);
					result->setRequestedUri(resourceUri);
					callSlot(result, slot);
					continue;
				}

				auto loader = ResourceLoaders::getInstance()->getLoader(uri.extension());
				if (!loader) {
					auto msg = sdk::utils::ReportMessage::create();
					msg->setTitle("Unable to load resource");
					msg->setMessage("No loader registered that can load such resource");
					msg->addInfoLine("Resource URI: {}", uri.string());
					result->setError(msg);
					result->setRequestedUri(resourceUri);
					callSlot(result, slot);
					continue;
				}

				std::shared_ptr<std::istream> stream;
				if (auto msg = application->getResourceBundle()->getResourceStream(uri, stream)) {
					msg->setTitle("Unable to load resource");
					result->setError(msg);
					result->setRequestedUri(resourceUri);
					callSlot(result, slot);
					continue;
				}
				std::shared_ptr<IResource> resource;
				if (auto msg = loader->load(this, stream, resource)) {
					msg->setTitle("Unable to load resource");
					result->setError(msg);
					result->setRequestedUri(resourceUri);
					callSlot(result, slot);
					continue;
				}
				result->setResource(resource);
				result->setRequestedUri(resourceUri);
				result->setState(ResourceLoadResult::State::LOADED);
				callSlot(result, slot);

				if (auto msg = loader->init(this, resource)) {
					msg->setTitle("Unable to load resource");
					result->setError(msg);
					result->setRequestedUri(resourceUri);
					callSlot(result, slot);
					continue;
				}
				resources->addResource(resourceUri, resource);
				result->setState(ResourceLoadResult::State::READY);
				callSlot(result, slot);
			} catch (...) {
				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Unable to load resource");
				msg->setMessage("Exception thrown while executing request");
				msg->addInfoLine("Resource URI: {}", uri.string());
				result->setError(msg);
				result->setRequestedUri(resourceUri);
				callSlot(result, slot);
			}
		}
		std::lock_guard lock(queueMutex);
		queue.clear();
	}
}

void ResourcesWindow::callSlot(const std::shared_ptr<ResourceLoadResult> &pResult,
							   const sigc::slot<void(const std::shared_ptr<ResourceLoadResult> &pResult)> &pSlot) {
	try {
		pSlot(pResult);
	} catch (...) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to send loading result to the callback");
		msg->setMessage("Exception thrown in callback");
		msg->addInfoLine("Result state: {}", pResult->getStateStr());
		msg->addInfoLine("Requested resource URI: {}", pResult->getRequestedUri());
		if (pResult->isErrored()) {
			msg->addInfoLine("Result error reported earlier");
			utils::Logger::error(pResult->getError());
		}
		utils::Logger::error(msg);
	}
}

} // namespace mer::sdk::main
#endif