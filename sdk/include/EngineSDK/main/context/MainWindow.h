//
// Created by alexus on 31.01.24.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef EDITOR_SDK
	#include "EngineSDK/main/resources/ResourceRequests.h"
	#include "EngineSDK/main/scene/IScene.h"
	#include "Window.h"

namespace n::sdk::main {
class ResourcesWindow;
class LoadedResources;
class IScene;

class MainWindow : public Window {
	std::shared_ptr<LoadedResources> loadedResources;
	std::shared_ptr<ResourcesWindow> resourcesWindow;
	std::shared_ptr<IScene> currentScene;

protected:
	MainWindow();

public:
	static auto create() { return std::shared_ptr<MainWindow>(new (std::nothrow) MainWindow()); }

	engine::utils::ReportMessagePtr openScene(const std::shared_ptr<IScene> &pNewScene);

	void runMainLoop();

private:
	void onResize(int pWidth, int pHeight) override;
};
} // namespace n::sdk::main

#endif

#endif //MAINWINDOW_H
