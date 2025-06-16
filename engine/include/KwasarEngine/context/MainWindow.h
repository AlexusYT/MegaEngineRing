//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 31.01.24.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef EDITOR_SDK
#include "KwasarEngine/scene/IScene.h"
#include "Window.h"

namespace mer::sdk {
class IApplication;
class ResourcesWindow;
class LoadedResources;
class IScene;

class MainWindow : public Window {
	std::shared_ptr<LoadedResources> loadedResources;
	std::shared_ptr<ResourcesWindow> resourcesWindow;
	std::shared_ptr<IScene> currentScene;
	IApplication* application;

protected:
	MainWindow();

public:
	static auto create() { return std::shared_ptr<MainWindow>(new(std::nothrow) MainWindow()); }

	sdk::ReportMessagePtr openScene(const std::shared_ptr<IScene> &pNewScene);

	void runMainLoop();

	[[nodiscard]] IApplication* getApplication() const { return application; }

	void setApplication(IApplication* pApplication) { application = pApplication; }

private:
	void onSizeChanged(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(int pKey, int pScancode, int pAction, int pMods) override;
};
} // namespace mer::sdk

#endif

#endif //MAINWINDOW_H