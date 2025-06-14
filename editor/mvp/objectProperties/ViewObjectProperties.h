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
// Created by alexus on 17.01.24.
//

#ifndef OBJECTPROPERTIESWINDOW_H
#define OBJECTPROPERTIESWINDOW_H

#include "EngineSDK/extensions/LightExtension.h"
#include "mvp/IView.h"
#include "mvp/editor/Editor.h"

namespace std {
struct type_index;
}

namespace mer::sdk {
class Light;
class MeshExtension;
class Scene3D;
class Node;
} // namespace mer::sdk

namespace mer::editor::mvp {
class IPresenterObjectProperties;

class IViewObjectProperties : public IView {
public:
	virtual void setPresenter(IPresenterObjectProperties* pPresenter) = 0;

	virtual void setSelectedNode(sdk::Node* pSelectedNode) = 0;

	virtual void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) = 0;
};

class ViewObjectProperties : public IViewObjectProperties, public EditorTool {
	static std::unordered_map<std::type_index, std::function<ImGuiID(const std::shared_ptr<sdk::Extension> &pExt)>>
	extRenderers;
	std::shared_ptr<IWidgetContext> context;
	IPresenterObjectProperties* presenter{};
	sdk::Node* selectedNode{};
	std::shared_ptr<sdk::Scene3D> scene{};

public:
	ViewObjectProperties(const std::shared_ptr<IWidgetContext> &pContext);

	void onUpdate(bool pVisible) override;

	void setPresenter(IPresenterObjectProperties* pPresenter) override { presenter = pPresenter; }

	void setSelectedNode(sdk::Node* pSelectedNode) override { selectedNode = pSelectedNode; }

	void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) override { scene = pScene; }

	[[nodiscard]] std::string getTitle() const override { return trs("ObjectPropertiesTool"); }

private:
	void openView() override;

	void closeView() override;

	void drawTransformation();

	ImGuiID drawMeshTab(const std::shared_ptr<sdk::MeshExtension> &pExt);

	static bool confirmationMenuItem(const char* pLabel, const char* pShortcut = nullptr, float pTimeout = 0.5f,
									 bool pSelected = false, bool pEnabled = true);

	ImGuiID drawLightTab(const std::shared_ptr<sdk::LightExtension> &pExt);

	void drawLightSourceSettings(const std::shared_ptr<sdk::Light> &pLight);

	static void getSpeeds(float &pSpeed, const char*&pRoundFormat) {
		pSpeed = 0.005f;
		pRoundFormat = "%.2f";
		if (ImGui::IsWindowFocused()) {
			auto ctrlDown = ImGui::IsKeyDown(ImGuiMod_Ctrl);
			auto shiftDown = ImGui::IsKeyDown(ImGuiMod_Shift);
			if (ctrlDown && !shiftDown) { //Only ctrl is pressed
				pRoundFormat = "%0.0f";
				pSpeed = 0.01f;
			} else if (!ctrlDown && shiftDown) { //Only shift is pressed
				pRoundFormat = "%0.3f";
				pSpeed = 0.0005f;
			} else if (ctrlDown && shiftDown) { //Both ctrl and shift are pressed
				pRoundFormat = "%0.1f";
				pSpeed = 0.005f;
			}
		}
	}

	enum class AngleUnit { RADIANS, DEGREES };

	static AngleUnit getAngleUnit() { return AngleUnit::DEGREES; }

	static std::string getAngleUnitStr() { return isDegrees() ? I18n::trAngleDegrees : std::string(""); }

	static bool isDegrees() { return getAngleUnit() == AngleUnit::DEGREES; }
};
} // namespace mer::editor::mvp


#endif //OBJECTPROPERTIESWINDOW_H
