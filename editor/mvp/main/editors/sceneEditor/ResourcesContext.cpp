//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

#include "EngineSDK/main/IApplication.h"
#include "EngineSDK/main/resources/LoadedResources.h"
#include "EngineSDK/main/resources/ResourceLoaders.h"
#include "Globals.h"

namespace mer::editor::mvp {
std::pair<std::shared_ptr<sdk::main::IResource>, sdk::utils::ReportMessagePtr> ResourcesContext::loadResourceSync(
	const std::string &pResourceUri) {

	std::promise<std::pair<std::shared_ptr<sdk::main::IResource>, sdk::utils::ReportMessagePtr>> promise;
	loadResourceAsync(pResourceUri, [&promise](const std::shared_ptr<sdk::main::IResource> &pResource,
											   const sdk::utils::ReportMessagePtr &pError) {
		auto result = std::make_pair(pResource, pError);
		promise.set_value(result);
	});
	auto future = promise.get_future();
	future.wait();
	return future.get();
}

void ResourcesContext::resourceLoop(const std::stop_token &pToken) {
	while (!pToken.stop_requested()) {
		sharedContext->make_current();
		std::unique_lock lck(waitMutex);
		cv.wait(lck, [this, pToken]() { return !queue.empty() || pToken.stop_requested(); });
		for (auto &[resourceUri, slot]: queue) {
			sdk::utils::ReportMessagePtr error;
			std::shared_ptr<sdk::main::IResource> resource = resources->getResource(resourceUri);
			if (!resource) {

				std::string uri;
				try {
					if (resourceUri.starts_with("_builtin_")) {
						auto uriPath = std::filesystem::path(resourceUri);
						auto path = Globals::getDataPath() / uriPath.lexically_relative("_builtin_").string();
						uri = path.string();
					} else {
						uri = resourceUri;
					}
					error = sdk::main::ResourceLoaders::getInstance()->load(this, application->getResourceBundle(), uri,
																			resource);
					if (resource) resources->addResource(resourceUri, resource);
				} catch (...) {
					error = sdk::utils::ReportMessage::create();
					error->setTitle("Failed to load resource");
					error->setMessage("Exception thrown while executing request");
					error->addInfoLine("Resource URI: {}", uri);
				}
			}
			try {
				slot(resource, error);
			} catch (...) {
				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to load resource");
				msg->setMessage("Exception thrown in callback");

				if (error) {
					sdk::utils::Logger::error(error);
					msg->addInfoLine("Additional error reported earlier");
				}
				sdk::utils::Logger::error(msg);
			}
		}
		std::lock_guard lock(queueMutex);
		queue.clear();
	}
}
} // namespace mer::editor::mvp