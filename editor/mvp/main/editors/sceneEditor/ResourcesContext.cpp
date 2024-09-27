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

#include "EngineSDK/main/resources/LoadedResources.h"

void mer::editor::mvp::ResourcesContext::resourceLoop(const std::stop_token &pToken) {
	while (!pToken.stop_requested()) {
		sharedContext->make_current();
		std::unique_lock lck(queueMutex);
		cv.wait(lck, [this, pToken]() { return !queue.empty() || pToken.stop_requested(); });
		for (auto &[request, slot]: queue) {
			sdk::utils::ReportMessagePtr error;
			std::shared_ptr<sdk::main::IResource> resource;
			try {
				error = resources->executeRequest(request, resource);
			} catch (...) {
				error = sdk::utils::ReportMessage::create();
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
					sdk::utils::Logger::error(error);
					msg->addInfoLine("Additional error reported earlier");
				}
				sdk::utils::Logger::error(msg);
			}
		}
		queue.clear();
	}
}