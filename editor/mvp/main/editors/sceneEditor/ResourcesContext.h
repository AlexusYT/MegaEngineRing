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

#ifndef RESOURCESCONTEXT_H
#define RESOURCESCONTEXT_H
#include <condition_variable>
#include <thread>

#include "EngineSDK/main/resources/IResources.h"

namespace mer::sdk::main {
class ILoadedResources;
}

namespace mer::editor::mvp {
class ResourcesContext : public sdk::main::IResources {
	std::list<std::pair<std::shared_ptr<sdk::main::ResourceRequest>, ResourceSlot>> queue;
	std::mutex queueMutex;
	std::shared_ptr<sdk::main::ILoadedResources> resources;
	std::condition_variable cv;

	std::shared_ptr<Gdk::GLContext> sharedContext;
	std::jthread thread;
	sdk::main::IApplication* application{};

public:
	explicit ResourcesContext(const std::shared_ptr<Gdk::GLContext> &pSharedContext)
		: sharedContext(pSharedContext), thread([this](const std::stop_token &pToken) { this->resourceLoop(pToken); }) {
		thread.detach();
	}

	~ResourcesContext() override {
		thread.request_stop();
		cv.notify_one();
	}

	void enqueueResourceLoading(const std::shared_ptr<sdk::main::ResourceRequest> &pRequest,
								const ResourceSlot &pSlot) override {

		std::lock_guard lock(queueMutex);
		queue.emplace_back(pRequest, pSlot);
		cv.notify_one();
	}

	void resourceLoop(const std::stop_token &pToken);

	[[nodiscard]] sdk::main::IApplication* getApplication() const override { return application; }

	void setApplication(sdk::main::IApplication* pApplication) override { application = pApplication; }

	void setResources(const std::shared_ptr<sdk::main::ILoadedResources> &pResources) { resources = pResources; }
};
} // namespace mer::editor::mvp


#endif //RESOURCESCONTEXT_H
