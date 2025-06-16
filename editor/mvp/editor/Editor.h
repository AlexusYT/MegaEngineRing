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

#ifndef EDITOR_H
#define EDITOR_H
#include "KwasarEngine/ui/UiBase.h"
#include "imgui.h"


struct ImGuiWindow;

namespace mer::editor::mvp {
class EditorUi;
class Editor;

class EditorTool : public std::enable_shared_from_this<EditorTool>, public sdk::UiBase {
	friend Editor;

protected:
	Editor* editor{};
	uint32_t dockspaceId{};

	explicit EditorTool(const std::string &pName) : UiBase(pName) {}

public:
	~EditorTool() override = default;

	void render() override;

	[[nodiscard]] Editor* getEditor() const { return editor; }

	void setEditor(Editor* pEditor) { editor = pEditor; }

	std::string getWindowName(uint32_t pDockspaceId) const {
		return std::format("{}###{}{:0>8X}", getTitle(), getName(), pDockspaceId);
	}

	virtual void onSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyChanged(int pKey, int pScancode, int pAction, int pMods);

	virtual void onMouseScroll(double pXOffset, double pYOffset);

	virtual void onMouseButton(int pButton, int pAction, int pMods);

	void setCurrentDockspace(ImGuiID pDockspaceId);
};

inline void EditorTool::onSizeChanged(const int /*pWidth*/, const int /*pHeight*/) {}

inline void EditorTool::onCursorPosChanged(const double /*pX*/, const double /*pY*/) {}

inline void EditorTool::onKeyChanged(const int /*pKey*/, const int /*pScancode*/, const int /*pAction*/,
									 const int /*pMods*/) {}

inline void EditorTool::onMouseScroll(const double /*pXOffset*/, const double /*pYOffset*/) {}

inline void EditorTool::onMouseButton(const int /*pButton*/, const int /*pAction*/, const int /*pMods*/) {}

class Editor {
	EditorUi* ui{};
	std::vector<std::shared_ptr<EditorTool>> tools;
	std::string name; // Document name
	uint32_t id;      // Document identifier (unique)
	uint32_t setDockId = 0;
	bool open = true;

	ImGuiWindow* window = nullptr;
	uint32_t currDockId = 0;
	uint32_t currLocationId = 0; // Current Dock node we are docked into _OR_ window ID if floating window
	uint32_t prevLocationId = 0; // Previous dock node we are docked into _OR_ window ID if floating window
	uint32_t prevDockspaceId = 0;
	ImGuiWindowClass
	toolWindowsClass;
	// All our tools windows will share the same WindowClass (based on ID) to avoid mixing tools from different top-level editor

	bool unsaved = false;

protected:
	uint32_t currDockspaceId =
		0; // Dockspace ID ~~ Hash of LocationID + DocType (with MYEDITOR_CONFIG_SAME_LOCATION_SHARE_LAYOUT=1)
public:
	virtual ~Editor() = default;

	Editor(const std::string &pName);

	void addTool(const std::shared_ptr<EditorTool> &pTool) {
		pTool->setEditor(this);
		tools.emplace_back(pTool);
	}

	void removeTool(EditorTool* pTool) {
		erase_if(tools, [pTool](const std::shared_ptr<EditorTool> &pToolIn) { return pToolIn.get() == pTool; });
		pTool->setEditor(nullptr);
	}

	void updateLocation(uint32_t pToplevelDockspaceId);

	void updateContents();

	virtual void updateUi() {}

	ImGuiID calcDockspaceId() const;

	void layoutPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize,
					  /*MyEditorLayoutPreset preset, */ ImGuiDir pPanelDir);

	void resetToolsVisibility();

	[[nodiscard]] bool isUnsaved() const { return unsaved; }

	void setUnsaved(bool pUnsaved) { unsaved = pUnsaved; }

	[[nodiscard]] EditorUi* getUi() const { return ui; }

	void setUi(EditorUi* pUi) { ui = pUi; }

	[[nodiscard]] const std::vector<std::shared_ptr<EditorTool>> &getTools() const { return tools; }

	virtual void customRender();

	virtual void onSizeChanged(int pWidth, int pHeight);

	virtual void onCursorPosChanged(double pX, double pY);

	virtual void onKeyChanged(int pKey, int pScancode, int pAction, int pMods);

	virtual void onMouseScroll(double pXOffset, double pYOffset);

	virtual void onMouseButton(int pButton, int pAction, int pMods);

protected:
	void copyLayout(ImGuiID pSrcDockspaceId, ImGuiID pDstDockspaceId);

	virtual const char* getType() const = 0;

	virtual void loadPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir pPanelDir) = 0;
};
} // namespace mer::editor::mvp

#endif //EDITOR_H