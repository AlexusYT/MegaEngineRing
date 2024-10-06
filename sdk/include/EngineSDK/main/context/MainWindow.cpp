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

utils::ReportMessagePtr MainWindow::openScene(const std::shared_ptr<IScene> &pNewScene) {

	if (currentScene) {
		auto oldScene = currentScene;
		currentScene.reset();
		oldScene->setApplication(nullptr);
	}

	pNewScene->setApplication(getApplication());
	resourcesWindow->setApplication(getApplication());
	pNewScene->setResourceExecutor(resourcesWindow.get());

	if (auto msg = pNewScene->initScene()) return msg;
	currentScene = pNewScene;
	return nullptr;
}

void MainWindow::runMainLoop() {
	onSizeChanged(getWidth(), getHeight());
	while (!isCloseRequest()) {
		getContext()->makeCurrent();
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