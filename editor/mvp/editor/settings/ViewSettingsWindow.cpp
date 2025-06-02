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
// Created by alexus on 23.05.25.
//

#include "ViewSettingsWindow.h"

#include "Settings.h"
#include "mvp/contexts/UiWindowContext.h"

namespace mer::editor::mvp {
void ViewSettingsWindow::openPopup() { ImGui::OpenPopup(getId()); }

void ViewSettingsWindow::onUpdate(bool pVisible) {

	if (!pVisible) {
		if (selectedCategory) selectedCategory.reset();
		if (!clonedCategories.empty()) clonedCategories.clear();
		return;
	}
	// Left
	{
		ImGui::BeginChild("left pane1", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
		auto &categories = Settings::getCategories();
		for (auto category: categories)
			if (ImGui::Selectable(category->getName(), selectedCategory == category)) {
				selectedCategory = category;
				auto categoryName = category->getName();
				if (auto iter = clonedCategories.find(categoryName); iter == clonedCategories.end()) {
					clonedCategories.emplace(categoryName, std::make_pair(category->clone(), category));
				}
			}

		ImGui::EndChild();
	}

	ImGui::SameLine();

	// Right
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("item view1",
						  ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us

		if (selectedCategory) {
			ImGui::TextUnformatted(selectedCategory->getName());
			auto &clonedCategory = clonedCategories.at(selectedCategory->getName());
			if (auto cat = std::dynamic_pointer_cast<GeneralCategory>(clonedCategory.first)) {}
			if (auto cat = std::dynamic_pointer_cast<OtherCategory>(clonedCategory.first)) {
				if (ImGui::Checkbox("DebugEnabled", &cat->debugEnabled)) updateChangedState();
			}
		}
		ImGui::EndChild();
		if (ImGui::Button("OK")) {
			if (settingsChanged) applyChanges();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); }
		ImGui::SameLine();
		ImGui::BeginDisabled(!settingsChanged);
		if (ImGui::Button("Apply")) { applyChanges(); }
		ImGui::EndDisabled();
		ImGui::EndGroup();
	}
}

void ViewSettingsWindow::openView() { context->add(this); }

void ViewSettingsWindow::closeView() { context->remove(); }

void ViewSettingsWindow::updateChangedState() {
	bool changed = false;
	for (const auto &[cloned, original]: clonedCategories | std::views::values) {
		if (!original->check(cloned)) {
			changed = true;
			break;
		}
	}
	settingsChanged = changed;
}

void ViewSettingsWindow::applyChanges() {
	for (auto &category: clonedCategories | std::views::values) { category.second->apply(category.first); }
	updateChangedState();
}
} // namespace mer::editor::mvp