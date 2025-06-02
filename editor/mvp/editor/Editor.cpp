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

#include "Editor.h"

#include "EditorUi.h"
#include "imgui_internal.h"

#define MYEDITOR_CONFIG_SAME_LOCATION_SHARE_LAYOUT 1
#define MYEDITOR_CONFIG_ONMERGE_OVERWRITE_WITH_SOURCE_LAYOUT 0

namespace mer::editor::mvp {


void EditorTool::render() {
	if (!open) return;

	auto toolWindowName = getWindowName(dockspaceId);

	constexpr ImGuiWindowFlags toolsSharedWindowFlags =
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNavFocus;
	bool toolVisible = ImGui::Begin(toolWindowName.c_str(), &open, 0 | toolsSharedWindowFlags);
	onUpdate(toolVisible);
	ImGui::End();
}

void EditorTool::setCurrentDockspace(ImGuiID pDockspaceId) { dockspaceId = pDockspaceId; }

Editor::Editor(const std::string &pName) {
	id = ImHashStr(name.c_str(), 0);
	name = pName;
}

void Editor::updateLocation(uint32_t pToplevelDockspaceId) {
	IM_ASSERT(this != NULL && open);
	IM_ASSERT(pToplevelDockspaceId != 0);

	// Top level editors can only be docked with each others
	ImGui::SetNextWindowClass(&ui->getTopLevelEditorWindowClass());
	if (setDockId != 0) {
		ImGui::SetNextWindowDockID(setDockId);
		setDockId = 0;
	} else {
		ImGui::SetNextWindowDockID(pToplevelDockspaceId, ImGuiCond_FirstUseEver);
	}

	// Create top level editor tab/window
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	if (unsaved) window_flags |= ImGuiWindowFlags_UnsavedDocument;
	//PushDocumentColors();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	/*bool visible =*/ImGui::Begin(name.c_str(), &open, window_flags);
	ImGui::PopStyleVar();
	//PopDocumentColors();

	// Store last focused document
	// This should ideally be a stack so we can handle closure immediately without an awkward gap where LastActiveDocument is NULL.
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_DockHierarchy))
		ui->setLastActiveEditor(this);

	// Set WindowClass based on per-document ID, so tabs from Document A are not dockable in Document B etc. We could be using any ID suiting us, e.g. &doc
	// We also set ParentViewportId to request the platform back-end to set parent/child relationship at the windowing level.
	window = ImGui::GetCurrentWindow();
	toolWindowsClass.ClassId = id;
	toolWindowsClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoTaskBarIcon | ImGuiViewportFlags_NoDecoration;
	toolWindowsClass.ParentViewportId = ImGui::GetWindowViewport()->ID; // Make child of the top-level editor window
	toolWindowsClass.FocusRouteParentWindowId = window->ID;				// Setup shortcut routing (#6798)
	toolWindowsClass.DockingAllowUnclassed = true;

	// Track LocationID change so we can fork/copy the layout data according to where the window is going + reference count
	// LocationID ~~ (DockId != 0 ? DockId : DocumentID) // When we are in a loose floating window we use our own document id instead of the dock id
	currDockId = ImGui::GetWindowDockID();
	prevLocationId = currLocationId;
	currLocationId = currDockId != 0 ? currDockId : id;

	// Dockspace ID ~~ Hash of LocationID + DocType
	// So all editors of a same type inside a same tab-bar will share the same layout.
	// We will also use this value as a suffix to create window titles, but we could perfectly have an indirection to allocate and use nicer names for window names (e.g. 0001, 0002).
	prevDockspaceId = currDockspaceId;
	currDockspaceId = calcDockspaceId();
	IM_ASSERT(currDockspaceId != 0);

	// Keep track of top level nodes
	if (currDockId != 0 && ui->topLevelDockNodeIds.contains(currDockId) == false)
		ui->topLevelDockNodeIds.push_back(currDockId);

	ImGui::End();
}

void Editor::updateContents() {
	// This is the second Begin(), as MyEditor_UpdateDocLocationAndLayout() has already done one
	// (Therefore only the p_open and flags of the first call to Begin() applies)
	ImGui::Begin(name.c_str());
	IM_ASSERT(ImGui::GetCurrentWindow()->BeginCount == 2);

	const ImGuiID dockspace_id = currDockspaceId;
	const ImVec2 dockspace_size = ImGui::GetContentRegionAvail();

	// Fork settings when extracting to a new location, or Overwrite settings when docking back into an existing location
	if (prevLocationId != 0 && prevLocationId != currLocationId) {
		// Count references to tell if we should Copy or Move the layout.
		int prevDockspaceRefCount = 0;
		int currDockspaceRefCount = 0;
		for (auto editor: ui->getEditors()) {
			if (editor->currDockspaceId == prevDockspaceId) prevDockspaceRefCount++;
			if (editor->currDockspaceId == currDockspaceId) currDockspaceRefCount++;
		}

		// Fork or overwrite settings
		// FIXME: should be able to do a "move window but keep layout" if curr_dockspace_ref_count > 1.
		IMGUI_DEBUG_LOG("LayoutCopy DockID %08X -> DockID %08X requested by doc '%s'\n", prevDockspaceId,
						currDockspaceId, name.c_str());
		IMGUI_DEBUG_LOG("--> prev_dockspace_ref_count = %d --> %s\n", prevDockspaceRefCount,
						(prevDockspaceRefCount == 0) ? "Remove" : "Keep");
		IMGUI_DEBUG_LOG("--> curr_dockspace_ref_count = %d\n", currDockspaceRefCount);

#if MYEDITOR_CONFIG_ONMERGE_OVERWRITE_WITH_SOURCE_LAYOUT
		// Always overwrite with dragged layout
		MyEditor_LayoutCopy(prevDockspaceId, currDockspaceId);
#else
		// Priority existing layout
		if (currDockspaceRefCount <= 1) copyLayout(prevDockspaceId, currDockspaceId);
#endif


		if (prevDockspaceRefCount == 0) {
			ImGui::DockBuilderRemoveNode(prevDockspaceId);

#if 1
			// Delete settings of old windows
			// Rely on window name to ditch their .ini settings forever..
			char window_suffix[16];
			ImFormatString(window_suffix, IM_ARRAYSIZE(window_suffix), "##%08X", prevDockspaceId);
			size_t window_suffix_len = strlen(window_suffix);
			ImGuiContext &g = *GImGui;
			for (ImGuiWindowSettings* settings = g.SettingsWindows.begin(); settings != NULL;
				 settings = g.SettingsWindows.next_chunk(settings)) {
				if (settings->ID == 0) continue;
				const char* window_name = settings->GetName();
				size_t window_name_len = strlen(window_name);
				if (window_name_len >= window_suffix_len)
					if (strcmp(window_name + window_name_len - window_suffix_len, window_suffix) == 0) // Compare suffix
						ImGui::ClearWindowSettings(window_name);
			}
#endif
		}
	} else if (ImGui::DockBuilderGetNode(currDockspaceId) == NULL) {
		// Initialize new dockspace with preset
		// (to be able to call DockBuilderSplitNode() we need a node size ahead, so any preset override will probably have to be applied between Begin and Dockspace calls)
		// FIXME: Use g_LastActiveDoc as source reference and do a copy?
		//MyEditor_LayoutPreset(doc, dockspace_id, dockspace_size, MyEditorLayoutPreset_Standard, ImGuiDir_Left);
		layoutPreset(dockspace_id, dockspace_size, ImGuiDir_Left);
	}

	// FIXME-DOCK: This is a little tricky to explain but we currently need this to use the pattern of sharing a same dockspace between tabs of a same tab bar
	bool visible = true;
#if MYEDITOR_CONFIG_SAME_LOCATION_SHARE_LAYOUT
	if (ImGui::GetCurrentWindow()->Hidden) visible = false;
#endif

	if (!visible) {
		// Keep alive document dockspace so windows that are docked into it but which visibility are not linked to the dockspace visibility won't get undocked.
		ImGui::DockSpace(dockspace_id, dockspace_size, ImGuiDockNodeFlags_KeepAliveOnly, &toolWindowsClass);
		ImGui::End();
		return;
	}
	// Submit the menu bar
	updateUi();
	//MyEditor_TopLevelMenuBar(editor, doc, dockspace_id, dockspace_size);
	//ImGui::Text("IsWindowFocused(_DockHierarchy|_ChidWindows): %d", ImGui::IsWindowFocused(ImGuiFocusedFlags_DockHierarchy | ImGuiFocusedFlags_ChildWindows));

	//if (ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S)) doc->Save();

	// Submit the dockspace node
	bool hasToolsOpened = false;
	for (auto tool: tools) { hasToolsOpened = tool->isOpen() || hasToolsOpened; }
	ImGuiContext &g = *GImGui;
	auto val = g.CurrentWindow->GetID("ToolsPopup");
	// Show a child window with the button that can add closed tools
	if (!hasToolsOpened) {
		glm::vec2 contentSize = ImGui::GetContentRegionAvail();
		glm::vec2 winPos = ImGui::GetWindowPos();
		ImGui::SetNextWindowPos(winPos + contentSize * 0.5f, 0, ImVec2(0.5, 0.5));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
		auto childVisible = ImGui::BeginChild(
			"NoTools", {0, 0}, ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
		ImGui::PopStyleVar();
		if (childVisible) {
			ImGui::TextWrapped("No opened tools in this workspace");

			auto text = "Select a tool to open...";
			glm::vec2 textSize = ImGui::CalcTextSize(text);
			if (ImGui::Button(text, textSize + glm::vec2(30, 30))) ImGui::OpenPopup(val);
		}
		ImGui::EndChild();
	}

	if (ImGui::BeginPopup("ToolsPopup")) {
		for (auto tool: tools) {
			if (ImGui::MenuItem(tool->getTitle().c_str(), nullptr, &tool->open))
				//We should create the dockspace when the tool opens
				hasToolsOpened = true;
		}
		ImGui::EndPopup();
	}
	// No need to create the dockspace if there are no any tools opened.
	if (hasToolsOpened) { ImGui::DockSpace(dockspace_id, dockspace_size, ImGuiDockNodeFlags_None, &toolWindowsClass); }
	ImGui::End();

	const bool isLastFocusedDoc = (ui->getLastActiveEditor() == this);

	// Submit tools
	for (auto editorTool: tools) {
		if (!editorTool->isOpen()) continue;

		editorTool->setCurrentDockspace(dockspace_id);
		auto toolWindowName = editorTool->getWindowName(dockspace_id);

		if (const ImGuiWindow* toolWindow = ImGui::FindWindowByName(toolWindowName.c_str())) {
			ImGuiDockNode* windowNode = toolWindow->DockNode;
			if (windowNode && ImGui::DockNodeBeginAmendTabBar(windowNode)) {
				if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) ImGui::OpenPopup(val);
				ImGui::SetItemTooltip("Open or close the workspace tools");
				ImGui::DockNodeEndAmendTabBar();
			}
			if (ui->isOptFloatingToolsOnlyVisibleWhenParentIsFocused() && !isLastFocusedDoc) {
				if (windowNode == nullptr && toolWindow->DockId != 0)
					windowNode = ImGui::DockContextFindNodeByID(ImGui::GetCurrentContext(), toolWindow->DockId);
				if (windowNode == nullptr || ImGui::DockNodeGetRootNode(windowNode)->ID != dockspace_id) continue;
			}
		}
		ImGui::SetNextWindowClass(&toolWindowsClass);
		editorTool->render();
	}
}

ImGuiID Editor::calcDockspaceId() const {
	ImU32 s = 0;
#if MYEDITOR_CONFIG_SAME_LOCATION_SHARE_LAYOUT
	// Shared dockspace per-document type in a same tab-bar
	// - Pros: Always sync by default.
	// - Cons: Ambiguous/weird merging/re-docking.
	s = currLocationId;
	s = ImHashStr(getType(), 0, s);
#else
	// Unique dockspace per document
	// - Cons: Not sync by default. Create more windows/data/garbage.
	s = id;
	s = ImHashStr("Dockspace", 0, s);
#endif
	return s;
}

void Editor::layoutPreset(ImGuiID pDockspaceId, ImVec2 pDockspaceSize, ImGuiDir pPanelDir) {

	// FIXME: Even though the call to DockBuilderRemoveNodeChildNodes will remove child nodes and move all their child windows to the root of the dockspace,
	// we won't be catching floating windows (e.g. any Tool Window moved outside) so we need to re-dock them explicitly..
	// Perhaps we could store some sort of link between those windows and our dockspace. Could this link be stored on imgui side?
	// Perhaps actually iterate all windows/settings (to find our dockspace_id marker in the name) and setup things based on that?
	// Until then we need an explicit knowledge of our windows.
	resetToolsVisibility();
	loadPreset(pDockspaceId, pDockspaceSize, pPanelDir);
	/*char buf[128];
	int buf_size = IM_ARRAYSIZE(buf);

	if (preset == MyEditorLayoutPreset_Flat) {
		ImGuiID dock_main_id = dockspace_id;
		ImGui::DockBuilderRemoveNodeChildNodes(dock_main_id);
		ImGui::DockBuilderDockWindow(
			MyEditor_GetToolWindowName(MyEditorToolType_Log, doc->CurrDockspaceID, buf, buf_size), dock_main_id);
		ImGui::DockBuilderDockWindow(
			MyEditor_GetToolWindowName(MyEditorToolType_Properties, doc->CurrDockspaceID, buf, buf_size), dock_main_id);
		ImGui::DockBuilderDockWindow(
			MyEditor_GetToolWindowName(MyEditorToolType_Mesh, doc->CurrDockspaceID, buf, buf_size), dock_main_id);
		ImGui::DockBuilderDockWindow(
			MyEditor_GetToolWindowName(MyEditorToolType_Extra, doc->CurrDockspaceID, buf, buf_size), dock_main_id);
		ImGui::DockBuilderFinish(dockspace_id);
	}

	if (preset == MyEditorLayoutPreset_Standard) {
		IM_ASSERT(pPanelDir == ImGuiDir_Left || pPanelDir == ImGuiDir_Right);

		ImGuiID dock_main_id = pDockspaceId;
		ImGui::DockBuilderAddNode(dock_main_id, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dock_main_id, pDockspaceSize);
		ImGuiID dock_id_prop = ImGui::DockBuilderSplitNode(dock_main_id, pPanelDir, 0.20f, NULL, &dock_main_id);
		ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

		ImGui::DockBuilderDockWindow(MyEditor_GetToolWindowName(MyEditorToolType_Log, doc->CurrDockspaceID, buf, buf_size),
									 dock_id_bottom);
		ImGui::DockBuilderDockWindow(
			MyEditor_GetToolWindowName(MyEditorToolType_Properties, doc->CurrDockspaceID, buf, buf_size), dock_id_prop);
		ImGui::DockBuilderDockWindow(MyEditor_GetToolWindowName(MyEditorToolType_Mesh, doc->CurrDockspaceID, buf, buf_size),
									 dock_id_prop);
		ImGui::DockBuilderDockWindow(
			MyEditor_GetToolWindowName(MyEditorToolType_Extra, doc->CurrDockspaceID, buf, buf_size), dock_id_prop);
		ImGui::DockBuilderFinish(pDockspaceId);
	}*/
}

void Editor::resetToolsVisibility() {
	for (auto editorTool: tools) { editorTool->setOpen(true); }
}

void Editor::copyLayout(ImGuiID pSrcDockspaceId, ImGuiID pDstDockspaceId) {
	IM_ASSERT(pSrcDockspaceId != 0);
	IM_ASSERT(pDstDockspaceId != 0);

	// Helper to build an array of strings pointer into the same contiguous memory buffer.
	struct ContiguousStringArrayBuilder {
		ImVector<char> Buf;
		ImVector<int> Offsets;

		void AddEntry(const std::string &pStr) { AddEntry(pStr.c_str(), pStr.size() + 1); }

		void AddEntry(const char* data, size_t data_len) {
			Offsets.push_back(Buf.Size);
			int offset = Buf.Size;
			Buf.resize(Buf.Size + (int) data_len);
			memcpy(Buf.Data + offset, data, data_len);
		}

		void BuildPointerArray(ImVector<const char*>* out_array) {
			out_array->resize(Offsets.Size);
			for (int n = 0; n < Offsets.Size; n++) (*out_array)[n] = Buf.Data + Offsets[n];
		}
	};

	// Build an array of remapped names
	ContiguousStringArrayBuilder name_pairs_builder;
#if 1
	// Iterate all known tools
	name_pairs_builder.Buf.resize(static_cast<int>(50 * tools.size()));
	for (auto tool: tools) {
		auto srcName = tool->getWindowName(pSrcDockspaceId);
		name_pairs_builder.AddEntry(srcName);
		auto destName = tool->getWindowName(pDstDockspaceId);
		name_pairs_builder.AddEntry(destName);
	}
#else
	// Iterate all possible windows, scan for ##XXXXXXXX marker and compare to our source dockspace id
	ImGuiContext &g = *GImGui;
	for (int window_n = 0; window_n < g.Windows.Size; window_n++) {
		ImGuiWindow* window = g.Windows[window_n];
		size_t window_name_len = strlen(window->Name);
		ImGuiID window_suffix = 0;
		if (window_name_len > 10 && strncmp(window->Name + window_name_len - 10, "##", 2) == 0)
			if (sscanf(window->Name + window_name_len - 8, "%08X", &window_suffix) == 1 &&
				window_suffix == src_dockspace_id) {
				name_pairs_builder.AddEntry(window->Name, window_name_len + 1);
				name_pairs_builder.AddEntry(window->Name, window_name_len + 1);
				ImFormatString(name_pairs_builder.Buf.Data + name_pairs_builder.Offsets.back() + window_name_len - 8,
							   8 + 1, "%08X", dst_dockspace_id);
			}
	}
#endif

	// Build the same array with char* pointers at it is the input of DockBuilderCopyDockspace() (may change its signature?)
	ImVector<const char*> window_remap_pairs;
	name_pairs_builder.BuildPointerArray(&window_remap_pairs);

	// Perform the cloning
	ImGui::DockBuilderCopyDockSpace(pSrcDockspaceId, pDstDockspaceId, &window_remap_pairs);
	ImGui::DockBuilderFinish(pDstDockspaceId);
}

void Editor::customRender() {
	for (const auto &tool: tools) tool->customRender();
}

void Editor::onSizeChanged(const int pWidth, const int pHeight) {
	for (const auto &tool: tools) tool->onSizeChanged(pWidth, pHeight);
}

void Editor::onCursorPosChanged(const double pX, const double pY) {
	for (const auto &tool: tools) tool->onCursorPosChanged(pX, pY);
}

void Editor::onKeyChanged(const int pKey, const int pScancode, const int pAction, const int pMods) {
	for (const auto &tool: tools) tool->onKeyChanged(pKey, pScancode, pAction, pMods);
}

void Editor::onMouseScroll(const double pXOffset, const double pYOffset) {
	for (const auto &tool: tools) tool->onMouseScroll(pXOffset, pYOffset);
}

void Editor::onMouseButton(const int pButton, const int pAction, const int pMods) {
	for (const auto &tool: tools) tool->onMouseButton(pButton, pAction, pMods);
}
} // namespace mer::editor::mvp