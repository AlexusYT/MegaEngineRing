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
// Created by alexus on 14.02.25.
//

#include "ConfirmationDialog.h"

namespace mer::editor::mvp {

std::shared_ptr<ConfirmationDialog> ConfirmationDialog::create(const std::string &pName, const std::string &pTitle) {
	return std::shared_ptr<ConfirmationDialog>(new ConfirmationDialog(pName, pTitle));
}

void ConfirmationDialog::updateUi() {
	ImGui::TextWrapped("%s", message.c_str());
	ImGui::Separator();


	for (int i = 0; auto button: buttons) {
		if (ImGui::Button(button.c_str(), ImVec2(120, 0))) {
			buttonClickedCallback(i);
			ImGui::CloseCurrentPopup();
		}
		if (i == 0) { ImGui::SetItemDefaultFocus(); }
		ImGui::SameLine();
		i++;
	}
	//if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
}
} // namespace mer::editor::mvp