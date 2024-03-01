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

protected:
	ResourcesWindow();

public:
	static auto create() { return std::shared_ptr<ResourcesWindow>(new (std::nothrow) ResourcesWindow()); }

	std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
											   const std::shared_ptr<IScene> &pScene) const;

	void enqueueResourceLoading(const std::shared_ptr<ResourceRequest> &pRequest, const ResourceSlot &pSlot) override;

	void requestStopThread();

private:
	void resourceLoop(const std::stop_token &pToken);
};
} // namespace mer::sdk::main


#endif
#endif //RESOURCESWINDOW_H
