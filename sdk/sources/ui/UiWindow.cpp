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

#include "EngineSDK/ui/UiWindow.h"

#include "EngineSDK/ui/UiPopup.h"

namespace mer::sdk {
void UiWindow::render() {
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	/*glm::vec2 actualPos;
	glm::vec2 actualPivot = pivot;
	if (position.x == POSITION_CENTERED) {
		actualPos.x = viewport->Size.x / 2.0f;
		if (pivot.x == 0.0f) actualPivot.x = 0.5;
	} else
		actualPos.x = position.x;

	if (position.y == POSITION_CENTERED) {
		actualPos.y = viewport->Size.y / 2.0f;
		if (pivot.y == 0.0f) actualPivot.y = 0.5;
	} else {
		actualPos.y = position.y;
	}

	ImGui::SetNextWindowPos(actualPos, 0, actualPivot);*/
	glm::vec2 actualSize;
	actualSize.x = size.x == SIZE_MATH_PARENT ? viewport->Size.x : size.x;
	actualSize.y = size.y == SIZE_MATH_PARENT ? viewport->Size.y : size.y;
	ImGui::SetNextWindowSize(actualSize);

	if (open) {
		if (ImGui::Begin(getTitle().c_str(), &open, windowFlags)) {
			for (auto uiPopup: popups) { uiPopup->render(); }
			onUpdate(true);
		}
		ImGui::End();
		onUpdate(false);
	} else
		onUpdate(false);
}

void UiWindow::addPopup(const std::shared_ptr<UiPopup> &pPopup) { popups.emplace_back(pPopup); }

void UiWindow::onSizeChanged(int /*pWidth*/, int /*pHeight*/) {}

void UiWindow::onCursorPosChanged(double /*pX*/, double /*pY*/) {}

void UiWindow::onKeyChanged(int /*pKey*/, int /*pScancode*/, int /*pAction*/, int /*pMods*/) {}

void UiWindow::onMouseScroll(double /*pXOffset*/, double /*pYOffset*/) {}

void UiWindow::onMouseButton(int /*pButton*/, int /*pAction*/, int /*pMods*/) {}
} // namespace mer::sdk