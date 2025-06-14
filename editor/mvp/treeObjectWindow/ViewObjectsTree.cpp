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

#include "EngineSDK/extensions/LightExtension.h"
#include "EngineSDK/extensions/MeshExtension.h"
#include "EngineSDK/gltf/Node.h"
#include "EngineSDK/scene/Scene3D.h"
#include "PresenterObjectsTree.h"
#include "imgui_internal.h"
#include "mvp/contexts/UiWindowContext.h"

namespace mer::editor::mvp {
ViewObjectsTree::ViewObjectsTree(const std::shared_ptr<IWidgetContext> &pContext)
	: EditorTool("TreeViewTool"), context(pContext) {}

void ViewObjectsTree::openView() { context->add(this); }

void ViewObjectsTree::closeView() { context->remove(); }

void ViewObjectsTree::onUpdate(bool pVisible) {
	if (!pVisible) return;
	if (!scene) {
		ImGui::Text("Programmer error. No Scene specified for %s", __FILE_NAME__);
		return;
	}
	const float textBaseWidth = ImGui::CalcTextSize("A").x;

	static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
										 ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY |
										 ImGuiTableFlags_ScrollX;
	if (ImGui::BeginTable("ObjectTreeTable", 2, table_flags)) {
		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
		ImGui::TableSetupColumn(tr("ObjectName"), ImGuiTableColumnFlags_NoResize);
		ImGui::TableSetupColumn(tr("ObjectType"), ImGuiTableColumnFlags_WidthFixed, textBaseWidth * 6.0f);
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableHeadersRow();

		hoveredNode = nullptr;
		updateTreeLevel(scene->getRootNodes());

		if (hoveredNode) {
			if (!nodeToReparent) {
				if (ImGui::IsMouseClicked(ImGuiPopupFlags_MouseButtonRight)) {
					ImGui::OpenPopup(contextMenuImguiId, ImGuiPopupFlags_MouseButtonRight);
					selectedNodeForContext = hoveredNode;
				} else if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonLeft)) {
					if (presenter) {
						presenter->clearSelection();
						presenter->select(hoveredNode);
					}
				}
			}
		}

		if (contextMenuImguiId == 0) contextMenuImguiId = ImGui::GetID("ContextMenu");
		if (ImGui::BeginPopup("ContextMenu")) {
			if (ImGui::MenuItem(tr("ObjectAdd"))) {
				auto newNode = sdk::Node::create(tr("ObjectUnnamed"));
				scene->addNode(selectedNodeForContext, newNode);
			}
			if (selectedNodeForContext) {
				if (ImGui::MenuItem(tr("ObjectReparent"))) { nodeToReparent = selectedNodeForContext; }
				if (ImGui::MenuItem(tr("ObjectRemove"))) {
					if (presenter) presenter->clearSelection();
					scene->removeNode(selectedNodeForContext);
				}
			}
			ImGui::EndPopup();
		} else
			selectedNodeForContext = nullptr;
		if (nodeToReparent) {
			if (ImGui::IsKeyReleased(ImGuiKey_Escape) || ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				nodeToReparent = nullptr;
			}

			std::string tooltipText;
			//translators: second line in reparent tooltip in an object tree window
			const std::string unformattedText = tr("Left click to reparent to the \"{}\".\n");
			if (hoveredNode && nodeToReparent
				!=
				hoveredNode
			) {
				tooltipText = std::vformat(unformattedText, std::make_format_args(hoveredNode->getName()));
				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					scene->reparentNode(nodeToReparent, hoveredNode);
					nodeToReparent = nullptr;
				}
			} else if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ForTooltip)) {
				//translators: root element in reparent tooltip in an object tree window
				auto root = trs("Root of the scene");
				tooltipText = std::vformat(unformattedText, std::make_format_args(root));

				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					scene->reparentNode(nodeToReparent, nullptr);
					nodeToReparent = nullptr;
				}
			}
			//translators: second line in reparent tooltip in an object tree window
			tooltipText += trs("Right click to cancel.");
			if (ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, ImGuiWindowFlags_None)) {
				ImGui::TextUnformatted(tooltipText.c_str());
				ImGui::EndTooltip();
			}
		} else {
			/*if (hoveredNode) {
				if (ImGui::BeginTooltipEx(ImGuiTooltipFlags_OverridePrevious, ImGuiWindowFlags_None)) {
					ImGui::TextUnformatted(hoveredNode->getName().c_str());
					ImGui::EndTooltip();
				}
			}*/

			if (!selectedNodeForContext && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
				ImGui::IsMouseClicked(ImGuiPopupFlags_MouseButtonRight)) {
				ImGui::OpenPopup(contextMenuImguiId, ImGuiPopupFlags_MouseButtonRight);
			}
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
		if (children.size() == 1) ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		bool nodeOpen = ImGui::TreeNodeEx(node->getName().c_str(), flags);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) { hoveredNode = node; }

		//ImGui::OpenPopupOnItemClick(id.c_str(), ImGuiPopupFlags_MouseButtonRight);
		ImGui::TableNextColumn();
		std::string types;
		for (auto &extension: node->getExtensions()) {
			if (extension.first == typeid(sdk::MeshExtension)) { types += "M "; } else if (
				extension.first == typeid(sdk::LightExtension)) { types += "L "; }
		}
		ImGui::TextUnformatted(types.c_str());

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
