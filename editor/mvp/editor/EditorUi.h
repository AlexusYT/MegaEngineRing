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
// Created by alexus on 29.03.25.
//

#ifndef EDITORUI_H
#define EDITORUI_H
#include "EngineSDK/scene/SceneUi.h"

namespace mer::editor::mvp {
class Editor;

class EditorUi : public sdk::SceneUi {
	std::vector<std::shared_ptr<Editor>> editors;
	Editor* lastActiveEditor{};
	ImGuiWindowClass topLevelEditorWindowClass;
	bool optFloatingToolsOnlyVisibleWhenParentIsFocused{true};

public:
	ImVector<ImGuiID> topLevelDockNodeIds; // Track active top level docking node to add a + in them

	EditorUi();

	void updateUi() override;


	void addEditor(const std::shared_ptr<Editor> &pEditor);

	size_t getEditorsCount() const { return editors.size(); }

	[[nodiscard]] const std::vector<std::shared_ptr<Editor>> &getEditors() const { return editors; }

	void setEditors(const std::vector<std::shared_ptr<Editor>> &pEditors) { editors = pEditors; }

	[[nodiscard]] Editor* getLastActiveEditor() const { return lastActiveEditor; }

	void setLastActiveEditor(Editor* pLastActiveEditor) { lastActiveEditor = pLastActiveEditor; }

	[[nodiscard]] bool isOptFloatingToolsOnlyVisibleWhenParentIsFocused() const {
		return optFloatingToolsOnlyVisibleWhenParentIsFocused;
	}

	void setOptFloatingToolsOnlyVisibleWhenParentIsFocused(bool pOptFloatingToolsOnlyVisibleWhenParentIsFocused) {
		optFloatingToolsOnlyVisibleWhenParentIsFocused = pOptFloatingToolsOnlyVisibleWhenParentIsFocused;
	}

	[[nodiscard]] const ImGuiWindowClass &getTopLevelEditorWindowClass() const { return topLevelEditorWindowClass; }

protected:
	void customRender() override;

	void onSizeChanged(int pWidth, int pHeight) override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(int pKey, int pScancode, int pAction, int pMods) override;

	void onMouseScroll(double pXOffset, double pYOffset) override;

	void onMouseButton(int pButton, int pAction, int pMods) override;
};

} // namespace mer::editor::mvp

#endif //EDITORUI_H
