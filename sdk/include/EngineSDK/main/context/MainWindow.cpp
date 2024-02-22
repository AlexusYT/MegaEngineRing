//
// Created by alexus on 31.01.24.
//

#ifndef EDITOR_SDK
	#include "MainWindow.h"

	#include <EngineUtils/utils/Logger.h>

	#include "ResourcesWindow.h"

namespace n::sdk::main {
MainWindow::MainWindow() {

	resourcesWindow = ResourcesWindow::create();
	resourcesWindow->setVisible(false);
	resourcesWindow->show();
	setSharedWindow(resourcesWindow);
}

engine::utils::ReportMessagePtr MainWindow::openScene(const std::shared_ptr<IScene> &pNewScene) {
	auto requests = std::make_shared<ResourceRequests>();
	if (auto msg = pNewScene->preloadScene(requests)) return msg;
	auto resources = resourcesWindow->executeRequests(requests, pNewScene);

	pNewScene->setResources(resources);

	if (auto msg = pNewScene->initScene()) return msg;
	currentScene = pNewScene;
	return nullptr;
}

void MainWindow::runMainLoop() {
	getContext()->makeCurrent();
	while (!isCloseRequest()) {
		if (currentScene) currentScene->render();
		getContext()->swapBuffers();
	}
}

void MainWindow::onResize(const int pWidth, const int pHeight) {
	if (currentScene) currentScene->resize(pWidth, pHeight);
}
} // namespace n::sdk::main
#endif