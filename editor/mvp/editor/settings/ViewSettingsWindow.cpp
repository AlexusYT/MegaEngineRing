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

#include "I18N.h"
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
		for (auto category: categories) {
			auto catName = category->getName();
			if (ImGui::Selectable(catName.c_str(), selectedCategory == category)) {
				selectedCategory = category;
				const auto &typeExpr = *category;
				if (auto iter = clonedCategories.find(typeid(typeExpr)); iter == clonedCategories.end()) {
					clonedCategories.emplace(typeid(typeExpr), std::make_pair(category->clone(), category));
				}
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
			ImGui::TextUnformatted(selectedCategory->getName().c_str());
			const auto &typeExpr = *selectedCategory;
			auto &clonedCategory = clonedCategories.at(typeid(typeExpr));
			if (auto cat = std::dynamic_pointer_cast<GeneralCategory>(clonedCategory.first)) {
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted(tr("InterfaceLanguage"));
				ImGui::SameLine();
				ImGui::SetNextItemWidth(-1);
				if (ImGui::BeginCombo("##language",
									  cat->language.empty() ? "" : I18n::trLanguagesMap.at(cat->language).c_str(), 0)) {
					for (const auto &type: I18n::trLanguages) {
						bool isSelected = cat->language == type;
						if (ImGui::Selectable(I18n::trLanguagesMap.at(type).c_str(), isSelected)) {
							cat->language = type;
							updateChangedState();
						}

						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::SetNextItemWidth(-1);
				if (ImGui::SliderFloat("##fontSize", &cat->fontSize, 8.0f, 30.0f, tr("FontSize: %.0f")))
					updateChangedState();
			}
			if (auto cat = std::dynamic_pointer_cast<OtherCategory>(clonedCategory.first)) {
				if (ImGui::Checkbox(tr("DebugEnabled"), &cat->debugEnabled)) updateChangedState();
			}
		}
		ImGui::EndChild();
		if (settingsChanged) {
			//translators: Button in settings window
			if (ImGui::Button(tr("ApplyAndClose"))) {
				if (settingsChanged) applyChanges();
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
		}

		if (settingsChanged) {
			//translators: Button in settings window
			if (ImGui::Button(tr("Apply"))) { applyChanges(); }
			ImGui::SameLine();
		}
		//translators: Button in settings window
		if (ImGui::Button(tr("Close"))) { ImGui::CloseCurrentPopup(); }
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
	if (auto msg = Settings::save()) { ke::Logger::error(msg); }
	updateChangedState();
}
} // namespace mer::editor::mvp
