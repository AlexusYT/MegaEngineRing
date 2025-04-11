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
// Created by alexus on 19.01.24.
//

#include "ViewObjectsTree.h"

#include <project/Project.h>

#include "EngineSDK/gltf/MeshInstance.h"
#include "EngineSDK/scene/Scene3D.h"
#include "PresenterObjectsTree.h"
#include "mvp/contexts/IWidgetContext.h"

namespace mer::editor::mvp {

ViewObjectsTree::ViewObjectsTree(const std::shared_ptr<IWidgetContext> &pContext)
	: EditorTool("TreeViewTool"), context(pContext) {}

void ViewObjectsTree::openView() { context->addTool(this); }

void ViewObjectsTree::closeView() { context->removeWidget(); }

void ViewObjectsTree::onUpdate(bool pVisible) {
	if (!pVisible) return;
	const float textBaseWidth = ImGui::CalcTextSize("A").x;

	static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
										 ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY |
										 ImGuiTableFlags_ScrollX;
	if (ImGui::BeginTable("ObjectTreeTable", 2, table_flags)) {
		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoResize);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, textBaseWidth * 6.0f);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		if (scene) updateTreeLevel(scene->getRootNodes());

		if (ImGui::BeginPopup("ContextMenu")) {
			ImGui::BeginDisabled();
			if (ImGui::MenuItem("Rename")) {}
			ImGui::SetItemTooltip("Not implemented yet");
			if (ImGui::MenuItem("Delete")) {}
			ImGui::SetItemTooltip("Not implemented yet");
			ImGui::EndDisabled();
			ImGui::EndPopup();
		}

		ImGui::EndTable();
	}
}

void ViewObjectsTree::updateTreeLevel(const std::vector<sdk::Node*> &pNodes) {
	for (auto node: pNodes) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		auto &children = node->getChildren();
		bool folder = !children.empty();

		ImGuiTreeNodeFlags flags =
			ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
		if (!folder) { flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; }
		auto iter = selectedMap.find(node);
		if (iter != selectedMap.end() && iter->second) flags |= ImGuiTreeNodeFlags_Selected;
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		bool nodeOpen = ImGui::TreeNodeEx(node->getName().c_str(), flags);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
			if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonRight)) {
				ImGui::OpenPopup("ContextMenu", ImGuiPopupFlags_MouseButtonRight);
				//selectedElement = node;
			} else if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonLeft)) {
				if (presenter) {
					presenter->clearSelection();
					presenter->select(node);
				}
			}
		}
		//ImGui::OpenPopupOnItemClick(id.c_str(), ImGuiPopupFlags_MouseButtonRight);
		ImGui::TableNextColumn();
		if (/*auto mesh =*/dynamic_cast<sdk::MeshInstance*>(node)) ImGui::TextUnformatted("Mesh");
		else
			ImGui::TextUnformatted("Node");

		if (nodeOpen && folder) {

			updateTreeLevel(children);
			ImGui::TreePop();
		}
	}
}

void ViewObjectsTree::setSceneToRender(const std::shared_ptr<sdk::Scene3D> &pScene) {
	scene = pScene;
	if (!scene) {
		selectedMap.clear();
		sceneNodesChangedConnection.disconnect();
		return;
	}
	sceneNodesChangedConnection = scene->connectOnNodeCollectionChanged([this]() {
		selectedMap.clear();
		for (auto node: scene->getNodes()) { selectedMap.emplace(node.get(), false); }
	});
}

void ViewObjectsTree::markSelected(const std::vector<sdk::Node*> &pNodes, bool pSelected) {
	for (auto node: pNodes) {
		if (auto iter = selectedMap.find(node); iter != selectedMap.end()) iter->second = pSelected;
	}
}

} // namespace mer::editor::mvp