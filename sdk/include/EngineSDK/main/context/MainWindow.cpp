//
// Created by alexus on 31.01.24.
//

#ifndef EDITOR_SDK
	#include "MainWindow.h"

	#include <GLFW/glfw3.h>

	#include "EngineSDK/utils/ModifierKeys.h"
	#include "ResourcesWindow.h"

namespace mer::sdk::main {
MainWindow::MainWindow() {

	resourcesWindow = ResourcesWindow::create();
	resourcesWindow->setVisible(false);
	resourcesWindow->show();
	setSharedWindow(resourcesWindow);
}

sdk::utils::ReportMessagePtr MainWindow::openScene(const std::shared_ptr<IScene> &pNewScene) {
	auto requests = std::make_shared<ResourceRequests>();
	if (auto msg = pNewScene->preloadScene(requests)) return msg;
	auto resources = resourcesWindow->executeRequests(requests, pNewScene);

	pNewScene->setResources(resources);

	if (auto msg = pNewScene->initScene()) return msg;
	currentScene = pNewScene;
	return nullptr;
}

void MainWindow::runMainLoop() {
	onSizeChanged(getWidth(), getHeight());
	getContext()->makeCurrent();
	while (!isCloseRequest()) {
		if (currentScene) currentScene->render();
		getContext()->swapBuffers();
	}
}

void MainWindow::onSizeChanged(const int pWidth, const int pHeight) {
	if (currentScene) currentScene->resize(pWidth, pHeight);
}

void MainWindow::onCursorPosChanged(double pX, double pY) {
	if (currentScene) currentScene->onCursorPosChanged(pX, pY);
}

void MainWindow::onKeyChanged(int /*pKey*/, int pScancode, const int pAction, const int pMods) {
	if (currentScene)
		currentScene->onKeyChanged(static_cast<utils::KeyboardKey>(pScancode), pAction != GLFW_RELEASE,
								   utils::ModifierKeys(static_cast<uint8_t>(pMods)));
}
} // namespace mer::sdk::main
#endif