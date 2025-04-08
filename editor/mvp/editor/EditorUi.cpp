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

#include "EditorUi.h"

#include "Editor.h"
#include "imgui_internal.h"

namespace mer::editor::mvp {
EditorUi::EditorUi() {
	// Unique identifier to match/filter top level editors with the top level dockspace. This could be a user enum.
	topLevelEditorWindowClass.ClassId = ImHashStr("MY_TOPLEVEL_EDITOR", 0);
	topLevelEditorWindowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoAutoMerge;
	topLevelEditorWindowClass.ViewportFlagsOverrideClear =
		ImGuiViewportFlags_NoDecoration | ImGuiViewportFlags_NoTaskBarIcon;
	topLevelEditorWindowClass.ParentViewportId = 0; // Top level window
	topLevelEditorWindowClass.DockingAllowUnclassed = false;
	topLevelEditorWindowClass.DockingAlwaysTabBar = true;
}

void EditorUi::updateUi() {

	SceneUi::updateUi();
	// Open default documents on startup
	/*if (g_Editor.Init) {
		for (int n = 0; n < IM_ARRAYSIZE(g_DummyAvailDocs); n++)
			if (g_DummyAvailDocs[n].OpenByDefault)
				g_Editor.OpenDocument(g_DummyAvailDocs[n].Type, g_DummyAvailDocs[n].Name, 0);
		g_Editor.Init = false;
	}
	if (g_Editor.DocsToOpen.Size > 0) {
		for (auto doc: g_Editor.DocsToOpen) g_Editor.Docs.push_back(doc);
		g_Editor.DocsToOpen.resize(0);
	}*/
	topLevelDockNodeIds.resize(0);

	// Top Level Dockspace
	ImGuiID root_dockspace_id = 0;
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When we have open documents, use a window without a title bar with a dockspace inside. Otherwise fake the dockspace tab-bar with a regular title bar.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		//PushDocumentColors();
		auto open_count = getEditorsCount();
		if (open_count > 0) ImGui::Begin("###DocumentRoot", NULL, flags | ImGuiWindowFlags_NoTitleBar);
		else
			ImGui::Begin("No document###DocumentRoot", NULL, flags | ImGuiWindowFlags_MenuBar);
		ImGui::PopStyleVar();

		root_dockspace_id = ImGui::GetID("MyRootDockspace");
		ImGui::DockSpace(root_dockspace_id, ImVec2(0, 0),
						 0
						 /*ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_AutoHideTabBar*/,
						 &topLevelEditorWindowClass);

		//ImGuiDockNode* node = ImGui::DockBuilderGetNode(root_dockspace_id);
		//node->IsHiddenTabBar = true;

		//PopDocumentColors();

		//if (open_count == 0) MyEditor_TopLevelMenuBar(&g_Editor, NULL, 0, ImVec2(0, 0));

		ImGui::End();
		ImGui::PopStyleVar();
	}
	/*for (auto [name, uiWindow]: windows) {
		ImGui::SetNextWindowClass(&topLevelEditorWindowClass);
		ImGui::SetNextWindowDockID(root_dockspace_id, ImGuiCond_FirstUseEver);
		uiWindow->render();
	}*/

	// Update the location of all documents
	// Submit Top Level Documents
	for (auto editor: editors) editor->updateLocation(root_dockspace_id);


	// Submit Top Level Documents
	for (auto editor: editors) {
		editor->updateContents();

		/*// Process closure
		if (doc->Open == false) {
			IM_DELETE(doc);
			if (g_Editor.LastActiveDocument == doc) g_Editor.LastActiveDocument = NULL;
			g_Editor.Docs.erase(g_Editor.Docs.Data + n);
			n--;
		}*/
	}

	// Demonstrate appending a + button into tab-bar
	// We collected a list of top-level node into TopLevelDockNodeIds[]
	// We could also iterate g.DockContext->Nodes[] and find active one where node->WindowClass == g_Editor.TopLevelEditorWindowClass...
	/*for (auto top_level_node_id: g_Editor.TopLevelDockNodeIds) {
		ImGuiDockNode* top_level_node = ImGui::DockBuilderGetNode(top_level_node_id);
		if (ImGui::DockNodeBeginAmendTabBar(top_level_node)) {
			if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) ImGui::OpenPopup("OpenDocument");
			if (ImGui::BeginPopup("OpenDocument")) {
				MyEditor_OpenDocumentSelector(&g_Editor);
				ImGui::EndPopup();
			}
			ImGui::DockNodeEndAmendTabBar();
		}
	}*/
}

void EditorUi::addEditor(const std::shared_ptr<Editor> &pEditor) { editors.emplace_back(pEditor)->setUi(this); }

void EditorUi::customRender() {
	for (const auto &editor: editors) editor->customRender();
}

void EditorUi::onSizeChanged(const int pWidth, const int pHeight) {
	for (const auto &editor: editors) editor->onSizeChanged(pWidth, pHeight);
}

void EditorUi::onCursorPosChanged(const double pX, const double pY) {
	for (const auto &editor: editors) editor->onCursorPosChanged(pX, pY);
}

void EditorUi::onKeyChanged(const int pKey, const int pScancode, const int pAction, const int pMods) {
	for (const auto &editor: editors) editor->onKeyChanged(pKey, pScancode, pAction, pMods);
}

void EditorUi::onMouseScroll(const double pXOffset, const double pYOffset) {
	for (const auto &editor: editors) editor->onMouseScroll(pXOffset, pYOffset);
}

void EditorUi::onMouseButton(const int pButton, const int pAction, const int pMods) {
	for (const auto &editor: editors) editor->onMouseButton(pButton, pAction, pMods);
}

} // namespace mer::editor::mvp