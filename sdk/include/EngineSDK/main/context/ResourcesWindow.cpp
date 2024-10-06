// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 02.02.24.
//

#include <condition_variable>
#include <future>

#include "EngineSDK/main/IApplication.h"
#include "EngineSDK/main/resources/ResourceLoaders.h"
#include "EngineUtils/utils/Logger.h"
#include "EngineUtils/utils/ReportMessage.h"
#ifndef EDITOR_SDK
	#include "ResourcesWindow.h"

	#include "EngineSDK/main/resources/LoadedResources.h"

namespace mer::sdk::main {
std::condition_variable cv;

ResourcesWindow::ResourcesWindow()
	: resources(LoadedResources::create()),
	  thread([this](const std::stop_token &pToken) { this->resourceLoop(pToken); }) {
	thread.detach();
}

std::pair<std::shared_ptr<IResource>, utils::ReportMessagePtr> ResourcesWindow::loadResourceSync(
	const std::string &pResourceUri) {

	std::lock_guard lock(queueMutex);
	std::promise<std::pair<std::shared_ptr<IResource>, utils::ReportMessagePtr>> promise;

	queue.emplace_back(pResourceUri,
					   [&promise](const std::shared_ptr<IResource> &pResource, const utils::ReportMessagePtr &pError) {
						   auto result = std::make_pair(pResource, pError);
						   promise.set_value(result);
					   });
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
			utils::ReportMessagePtr error;
			std::shared_ptr<IResource> resource = resources->getResource(resourceUri);
			if (!resource) {
				try {
					error = ResourceLoaders::getInstance()->load(this, application->getResourceBundle(), resourceUri,
																 resource);
					resources->addResource(resourceUri, resource);
				} catch (...) {
					error = utils::ReportMessage::create();
					error->setTitle("Failed to load resource");
					error->setMessage("Exception thrown while executing request");
				}
			}

			try {
				slot(resource, error);
			} catch (...) {
				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to load resource");
				msg->setMessage("Exception thrown in callback");

				if (error) {
					utils::Logger::error(error);
					msg->addInfoLine("Additional error reported earlier");
				}
				utils::Logger::error(msg);
			}
		}
		std::lock_guard lock(queueMutex);
		queue.clear();
	}
}


} // namespace mer::sdk::main
#endif