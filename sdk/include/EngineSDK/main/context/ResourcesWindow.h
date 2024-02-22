//
// Created by alexus on 02.02.24.
//

#ifndef RESOURCESWINDOW_H
#define RESOURCESWINDOW_H

#ifndef EDITOR_SDK
	#include "EngineSDK/main/resources/Resources.h"
	#include "EngineSDK/main/scene/IScene.h"
	#include "Window.h"

namespace n::sdk::main {
class ResourceRequests;
class ILoadedResources;
} // namespace n::sdk::main

namespace n::sdk::main {
class ResourcesWindow : public Window {

	std::shared_ptr<ILoadedResources> resources;

protected:
	ResourcesWindow();

public:
	static auto create() { return std::shared_ptr<ResourcesWindow>(new (std::nothrow) ResourcesWindow()); }

	std::shared_ptr<Resources> executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
											   const std::shared_ptr<IScene> &pScene) const;
};
} // namespace n::sdk::main


#endif
#endif //RESOURCESWINDOW_H
