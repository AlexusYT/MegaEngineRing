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

#include "EngineUtils/utils/UUID.h"
#include "imgui.h"
#include "imgui_internal.h"

//
// Created by alexus on 15.02.25.
//
namespace ImGui {
bool InputText(const char* pName, std::string &pText, ImGuiInputTextFlags pFlags) {
	return InputText(
		pName, pText.data(), pText.size() + 1, pFlags | ImGuiInputTextFlags_CallbackResize,
		[](ImGuiInputTextCallbackData* pData) {
			if (pData->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				const auto myStr = static_cast<std::string*>(pData->UserData);
				IM_ASSERT(myStr->data() == pData->Buf);
				myStr->resize(static_cast<std::string::size_type>(pData->BufTextLen));
				pData->Buf = myStr->data();
			}
			return 0;
		},
		&pText);
}

bool InputTextWithHint(const char* pName, const char* pHint, std::string &pText, ImGuiInputTextFlags pFlags) {

	return InputTextWithHint(
		pName, pHint, pText.data(), pText.size() + 1, pFlags | ImGuiInputTextFlags_CallbackResize,
		[](ImGuiInputTextCallbackData* pData) {
			if (pData->EventFlag == ImGuiInputTextFlags_CallbackResize) {
				const auto myStr = static_cast<std::string*>(pData->UserData);
				IM_ASSERT(myStr->data() == pData->Buf);
				myStr->resize(static_cast<std::string::size_type>(pData->BufTextLen));
				pData->Buf = myStr->data();
			}
			return 0;
		},
		&pText);
}

bool UuidText(const char* pName, const UUID &pUuid, int pFlags) {
	auto uuidText = pUuid.toString(true);

	ImGuiContext &g = *GImGui;
	PushStyleVar(ImGuiStyleVar_Alpha, g.Style.Alpha * g.Style.DisabledAlpha);
	bool result = InputText(pName, uuidText, pFlags | ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();
	return result;
}

bool DragFloat3(const char* pName, glm::vec3 &pVal, float pVSpeed, float pVMin, float pVMax, const char* pFormat,
				int pFlags) {
	float val[3] = {pVal[0], pVal[1], pVal[2]};
	if (DragFloat3(pName, val, pVSpeed, pVMin, pVMax, pFormat, pFlags)) {
		pVal = glm::vec3(val[0], val[1], val[2]);
		return true;
	}
	return false;
}

bool DragFloat4(const char* pName, glm::vec4 &pVal, float pVSpeed, float pVMin, float pVMax, const char* pFormat,
				int pFlags) {
	float val[4] = {pVal[0], pVal[1], pVal[2], pVal[3]};
	if (DragFloat4(pName, val, pVSpeed, pVMin, pVMax, pFormat, pFlags)) {
		pVal = glm::vec4(val[0], val[1], val[2], val[3]);
		return true;
	}
	return false;
}

void HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool InputTextErrored(const char* pName, const char* pHint, const char* pTooltipMsg, bool pErrored, std::string &pText,
					  ImGuiInputTextFlags pFlags) {
	bool result;
	if (pErrored) {
		PushStyleColor(ImGuiCol_Border, ImVec4(0.9f, 0.1f, 0.1f, 1.0f));
		result = InputTextWithHint(pName, pHint, pText, pFlags);
		SetItemTooltip("%s", pTooltipMsg);

		PopStyleColor();
	} else
		result = InputTextWithHint(pName, pHint, pText, pFlags);
	return result;
}
} // namespace ImGui