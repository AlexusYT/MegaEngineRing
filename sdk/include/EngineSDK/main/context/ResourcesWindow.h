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

#ifndef RESOURCESWINDOW_H
#define RESOURCESWINDOW_H

#ifndef EDITOR_SDK
	#include "EngineSDK/main/resources/IResources.h"
	#include "EngineSDK/main/resources/Resources.h"
	#include "EngineSDK/main/scene/IScene.h"
	#include "Window.h"
	#include <thread>

namespace mer::sdk::main {
class ResourceRequests;
class ILoadedResources;
} // namespace mer::sdk::main

namespace mer::sdk::main {
class ResourcesWindow : public Window, public IResources {
	std::list<std::pair<std::shared_ptr<ResourceRequest>, ResourceSlot>> queue;
	std::mutex queueMutex;

	std::shared_ptr<ILoadedResources> resources;
	std::jthread thread;
	IApplication* application;

protected:
	ResourcesWindow();

public:
	static auto create() { return std::shared_ptr<ResourcesWindow>(new (std::nothrow) ResourcesWindow()); }

	std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
											   const std::shared_ptr<IScene> &pScene) const;

	void enqueueResourceLoading(const std::shared_ptr<ResourceRequest> &pRequest, const ResourceSlot &pSlot) override;

	void requestStopThread();

	[[nodiscard]] IApplication* getApplication() const override { return application; }

	void setApplication(IApplication* pApplication) override { application = pApplication; }

private:
	void resourceLoop(const std::stop_token &pToken);
};
} // namespace mer::sdk::main


#endif
#endif //RESOURCESWINDOW_H
