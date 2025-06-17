//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 10.02.25.
//

#include "KwasarEngine/scene/SceneUi.h"

#include "KwasarEngine/context/Window.h"
#include "KwasarEngine/ui/UiWindow.h"
#include "imgui.h"

namespace ke {
void SceneUi::beforeUiFrame() {}

void SceneUi::updateUi() {
	//const ImGuiViewport* viewport = ImGui::GetMainViewport();
	//ImGui::DockSpaceOverViewport(0, viewport);
	for (auto [name, uiWindow]: windows) { uiWindow->render(); }
	for (auto [name, uiWindow]: popups) { uiWindow->render(); }
}

void SceneUi::customRender() { for (auto [name, uiWindow]: windows) { uiWindow->customRender(); } }

ImGuiIO &SceneUi::getIo() { return ImGui::GetIO(); }

void SceneUi::addUiWindow(UiWindow* pWindow) { windows.emplace(pWindow->getName(), pWindow); }

void SceneUi::removeUiWindow(const std::string &pName) { windows.erase(pName); }

void SceneUi::addUiPopup(UiPopup* pWindow) { popups.emplace(pWindow->getName(), pWindow); }

void SceneUi::removeUiPopup(const std::string &pName) { popups.erase(pName); }

void SceneUi::onSizeChanged(int pWidth, int pHeight) {
	for (auto win: windows) win.second->onSizeChanged(pWidth, pHeight);
}

void SceneUi::onCursorPosChanged(double pX, double pY) {
	for (auto win: windows) win.second->onCursorPosChanged(pX, pY);
}

void SceneUi::onKeyChanged(int pKey, int pScancode, int pAction, int pMods) {
	for (auto win: windows) win.second->onKeyChanged(pKey, pScancode, pAction, pMods);
}

void SceneUi::onMouseScroll(double pXOffset, double pYOffset) {
	for (auto win: windows) win.second->onMouseScroll(pXOffset, pYOffset);
}

void SceneUi::onMouseButton(int pButton, int pAction, int pMods) {
	for (auto win: windows) win.second->onMouseButton(pButton, pAction, pMods);
}

ReportMessagePtr SceneUi::onInitialize() { return Initializable::onInitialize(); }

void SceneUi::onUninitialize() {}
} // namespace ke