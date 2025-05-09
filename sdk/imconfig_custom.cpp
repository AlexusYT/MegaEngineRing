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

bool UuidText(const char* pName, const mer::sdk::UUID &pUuid, int pFlags) {
	auto uuidText = pUuid.toString(true);

	ImGuiContext &g = *GImGui;
	PushStyleVar(ImGuiStyleVar_Alpha, g.Style.Alpha * g.Style.DisabledAlpha);
	bool result = InputText(pName, uuidText, pFlags | ImGuiInputTextFlags_ReadOnly);
	PopStyleVar();
	return result;
}

static const float DRAG_MOUSE_THRESHOLD_FACTOR =
	0.50f; // Multiplier for the default value of io.MouseDragThreshold to make DragFloat/DragInt react faster to mouse drags.

bool DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* roundFormat,
			   const char* displayFormat, ImGuiSliderFlags flags) {
	return DragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, roundFormat, displayFormat, flags);
}

bool DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed, const void* p_min,
				const void* p_max, const char* roundFormat, const char* displayFormat, ImGuiSliderFlags flags) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems) return false;

	ImGuiContext &g = *GImGui;
	const ImGuiStyle &style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos,
						  window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
	const ImRect total_bb(
		frame_bb.Min,
		frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0)) return false;

	// Default format string when passing NULL
	if (roundFormat == NULL) roundFormat = DataTypeGetInfo(data_type)->PrintFmt;
	if (displayFormat == NULL) displayFormat = DataTypeGetInfo(data_type)->PrintFmt;

	const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.ItemFlags);
	bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
	if (!temp_input_is_active) {
		// Tabbing or CTRL-clicking on Drag turns it into an InputText
		const bool clicked = hovered && IsMouseClicked(0, ImGuiInputFlags_None, id);
		const bool double_clicked = (hovered && g.IO.MouseClickedCount[0] == 2 && TestKeyOwner(ImGuiKey_MouseLeft, id));
		const bool make_active = (clicked || double_clicked || g.NavActivateId == id);
		if (make_active && (clicked || double_clicked)) SetKeyOwner(ImGuiKey_MouseLeft, id);
		if (make_active && temp_input_allowed)
			if ((clicked && g.IO.KeyCtrl) || double_clicked ||
				(g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
				temp_input_is_active = true;

		// (Optional) simple click (without moving) turns Drag into an InputText
		if (g.IO.ConfigDragClickToInputText && temp_input_allowed && !temp_input_is_active)
			if (g.ActiveId == id && hovered && g.IO.MouseReleased[0] &&
				!IsMouseDragPastThreshold(0, g.IO.MouseDragThreshold * DRAG_MOUSE_THRESHOLD_FACTOR)) {
				g.NavActivateId = id;
				g.NavActivateFlags = ImGuiActivateFlags_PreferInput;
				temp_input_is_active = true;
			}

		// Store initial value (not used by main lib but available as a convenience but some mods e.g. to revert)
		if (make_active) memcpy(&g.ActiveIdValueOnActivation, p_data, DataTypeGetInfo(data_type)->Size);

		if (make_active && !temp_input_is_active) {
			SetActiveID(id, window);
			SetFocusID(id, window);
			FocusWindow(window);
			g.ActiveIdUsingNavDirMask = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
		}
	}

	if (temp_input_is_active) {
		// Only clamp CTRL+Click input when ImGuiSliderFlags_ClampOnInput is set (generally via ImGuiSliderFlags_AlwaysClamp)
		bool clamp_enabled = false;
		if ((flags & ImGuiSliderFlags_ClampOnInput) && (p_min != NULL || p_max != NULL)) {
			const int clamp_range_dir = (p_min != NULL && p_max != NULL)
											? DataTypeCompare(data_type, p_min, p_max)
											: 0; // -1 when *p_min < *p_max, == 0 when *p_min == *p_max
			if (p_min == NULL || p_max == NULL || clamp_range_dir < 0) clamp_enabled = true;
			else if (clamp_range_dir == 0)
				clamp_enabled =
					DataTypeIsZero(data_type, p_min) ? ((flags & ImGuiSliderFlags_ClampZeroRange) != 0) : true;
		}
		return TempInputScalar(frame_bb, id, label, data_type, p_data, roundFormat, clamp_enabled ? p_min : NULL,
							   clamp_enabled ? p_max : NULL);
	}

	// Draw frame
	const ImU32 frame_col = GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive
										: hovered		 ? ImGuiCol_FrameBgHovered
														 : ImGuiCol_FrameBg);
	RenderNavCursor(frame_bb, id);
	RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

	// Drag behavior
	const bool value_changed = DragBehavior(id, data_type, p_data, v_speed, p_min, p_max, roundFormat, flags);
	if (value_changed) MarkItemEdited(id);

	// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
	char value_buf[64];
	const char* value_buf_end =
		value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, displayFormat);
	if (g.LogEnabled) LogSetNextTextDecoration("{", "}");
	RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

	if (label_size.x > 0.0f)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label,
								g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
	return value_changed;
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