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

#include "ViewProjectExplorer.h"

#include <project/Project.h>

#include "EngineSDK/ui/UiWindow.h"
#include "IPresenterProjectExplorer.h"
#include "entries/ProjectExplorerElement.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/dialogs/confirmationDialog/ConfirmationDialog.h"

namespace mer::editor::mvp {
ViewProjectExplorer::ViewProjectExplorer(const std::shared_ptr<IWidgetContext> &pContext)
	: sdk::UiWindow("ViewProjectExplorer"), context(pContext) {
	dialog = ConfirmationDialog::create("ProjectExplorer-confirm", "ConfirmDeletion");
	dialog->setWindowFlags(ImGuiWindowFlags_NoResize);
	/*addPopup(dialog);
	setWindowFlags(ImGuiWindowFlags_MenuBar);*/
}

void ViewProjectExplorer::setElements(const std::vector<std::shared_ptr<ProjectExplorerElement>> &pArray) {
	array = pArray;
}

void ViewProjectExplorer::openView() { /*context->addWindow(this); */
}

void ViewProjectExplorer::closeView() { context->remove(); }

void ViewProjectExplorer::onUpdate(bool pVisible) {
	if (!pVisible) return;
	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	static ImGuiTableFlags table_flags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
	if (ImGui::BeginTable("3ways", 2, table_flags)) {
		// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
		ImGui::TableHeadersRow();

		updateTreeLevel(array);

		if (ImGui::BeginPopup("Menu123")) {
			if (ImGui::BeginMenu("New Resource")) {
				if (ImGui::MenuItem("Model")) {}
				if (ImGui::MenuItem("Material")) {}
				if (ImGui::MenuItem("Texture")) {}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("New Prefab")) {}
			if (ImGui::MenuItem("New Folder")) {}
			if (ImGui::MenuItem("New Scene")) {
				if (selectedElement && presenter) presenter->createScene(selectedElement->getPath());
			}
			if (ImGui::MenuItem("New Script")) {}
			ImGui::Separator();
			ImGui::BeginDisabled();
			if (ImGui::MenuItem("Rename")) {}
			ImGui::SetItemTooltip("Not implemented yet");
			ImGui::EndDisabled();
			if (ImGui::MenuItem("Delete")) {
				ImGui::OpenPopup(dialog->getId(), 0);
				//b = true;
				if (selectedElement && presenter) { presenter->deleteFile(selectedElement->getPath()); }
			}
			ImGui::Separator();
			ImGui::BeginDisabled();
			if (ImGui::MenuItem("Show in Files")) {
				//if (selectedElement && presenter) { presenter->openInFilesystem(selectedElement->getPath()); }
			}
			ImGui::SetItemTooltip("Not implemented yet");
			ImGui::EndDisabled();
			ImGui::EndPopup();
		}

		ImGui::EndTable();
	}
	/*if (ImGui::BeginPopupModal("Delete?11###ProjectExplorer-delete", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}*/
}

void ViewProjectExplorer::startConfirmation(const std::string &pTitle, const std::string &pMessage,
											const std::function<void(int pId)> &pResult,
											const std::vector<std::string> &pButtons) {
	dialog->setTitle(pTitle);
	dialog->setMessage(pMessage);
	dialog->setButtons(pButtons);
	dialog->setButtonClickedCallback(pResult);
}

void ViewProjectExplorer::updateTreeLevel(const std::vector<std::shared_ptr<ProjectExplorerElement>> &pElements) {
	for (uint32_t i = 0; i < pElements.size(); ++i) {
		auto node = pElements.at(i);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		bool folder = !node->getChildren().empty();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAllColumns;
		if (!folder) { flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; }
		bool visible = ImGui::TreeNodeEx(node->getName().c_str(), flags);
		if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonRight) &&
			ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup)) {
			ImGui::OpenPopup("Menu123", ImGuiPopupFlags_MouseButtonRight);
			selectedElement = node;
		}
		//ImGui::OpenPopupOnItemClick(id.c_str(), ImGuiPopupFlags_MouseButtonRight);
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(node->getTypeStr());
		if (visible && folder) {
			updateTreeLevel(node->getChildren());
			ImGui::TreePop();
		}
	}
}
} // namespace mer::editor::mvp
