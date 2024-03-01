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

std::shared_ptr<Resources> ResourcesWindow::executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
															const std::shared_ptr<IScene> &pScene) const {
	return resources->executeRequests(pRequests, pScene);
}

void ResourcesWindow::enqueueResourceLoading(const std::shared_ptr<ResourceRequest> &pRequest,
											 const ResourceSlot &pSlot) {
	{
		std::lock_guard lock(queueMutex);
		queue.emplace_back(pRequest, pSlot);
	}
	cv.notify_one();
}

void ResourcesWindow::requestStopThread() { thread.request_stop(); }

void ResourcesWindow::resourceLoop(const std::stop_token &pToken) {
	while (!pToken.stop_requested()) {
		std::unique_lock lck(queueMutex);
		cv.wait(lck, [this, pToken]() {
				return !queue.empty() || pToken.stop_requested(); });
		getContext()->makeCurrent();
		for (auto &[request, slot]: queue) {
			utils::ReportMessagePtr error;
			std::shared_ptr<IResource> resource;
			try {
				resource = resources->executeRequest(request, error);
			} catch (...) {
				error = utils::ReportMessage::create();
				error->setTitle("Failed to load resource");
				error->setMessage("Exception thrown while executing request");
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
		queue.clear();
	}
}


} // namespace mer::sdk::main
#endif