//
// Created by alexus on 02.02.24.
//

#ifndef EDITOR_SDK
	#include "ResourcesWindow.h"

	#include "EngineSDK/main/resources/LoadedResources.h"

namespace mer::sdk::main {
ResourcesWindow::ResourcesWindow() : resources(LoadedResources::create()) {}

std::shared_ptr<Resources> ResourcesWindow::executeRequests(const std::shared_ptr<ResourceRequests> &pRequests,
															const std::shared_ptr<IScene> &pScene) const {
	return resources->executeRequests(pRequests, pScene);
}
} // namespace mer::sdk::main
#endif