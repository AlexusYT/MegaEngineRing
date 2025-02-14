//  MegaEngineRing is a program that can speed up game development.
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

#include "EngineSDK/scene/SceneUi.h"

#include "EngineSDK/context/Window.h"
#include "EngineSDK/ui/UiWindow.h"
#include "imgui.h"

namespace mer::sdk {

void SceneUi::updateUi() {
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::ShowDemoWindow();
	ImGui::DockSpaceOverViewport(0, viewport);
	for (auto [name, uiPopup]: popups) { uiPopup->render(); }
	for (auto [name, uiWindow]: windows) { uiWindow->render(); }
}

void SceneUi::customRender() {}

ImGuiIO &SceneUi::getIo() { return ImGui::GetIO(); }

void SceneUi::addUiWindow(UiWindow* pWindow) { windows.emplace(pWindow->getName(), pWindow); }

void SceneUi::addUiPopup(const std::shared_ptr<UiPopup> &pPopup) { popups.emplace(pPopup->getName(), pPopup); }

void SceneUi::removeUiWindow(const std::string &pName) { windows.erase(pName); }

ReportMessagePtr SceneUi::onInitialize() { return Initializable::onInitialize(); }

void SceneUi::onUninitialize() {}
} // namespace mer::sdk