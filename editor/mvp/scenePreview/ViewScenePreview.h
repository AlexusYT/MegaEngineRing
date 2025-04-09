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
// Created by alexus on 27.02.25.
//

#ifndef VIEWSCENEPREVIEW_H
#define VIEWSCENEPREVIEW_H
#include "IViewScenePreview.h"
#include "mvp/editor/Editor.h"

namespace mer::sdk {
class BoundingVolumeProgram;
class VolumeAabb;
class Scene3D;
} // namespace mer::sdk

namespace mer::sdk {
class ProgramWideShaderBuffer;
class OrbitCameraExtension;
class Framebuffer;
} // namespace mer::sdk

namespace mer::editor::mvp {
class SceneOverlayElements;
class IWidgetContext;

class ViewScenePreview : public IViewScenePreview, public EditorTool {

	IPresenterScenePreview* presenter{};
	std::shared_ptr<IWidgetContext> context;
	std::shared_ptr<sdk::Framebuffer> framebuffer;
	bool frameDisplayed{};
	std::shared_ptr<sdk::OrbitCameraExtension> camera;
	std::shared_ptr<sdk::ProgramWideShaderBuffer> programBuffer;
	std::shared_ptr<SceneOverlayElements> overlay;
	bool widgetHovered{};
	bool mouseHeld{};
	bool moveKeyHeld{};
	glm::vec3 lastTargetPos{};
	glm::vec2 lastMousePos{};
	glm::vec2 mousePos{};
	float zoom{1};
	float targetZoom{5};

	std::optional<glm::dvec2> lastCursorPosMove{};
	std::optional<glm::dvec2> lastCursorPos{};
	bool rotate{};
	bool focusWindow{};


protected:
	ViewScenePreview(const std::string &pName, const std::shared_ptr<IWidgetContext> &pContext);

public:
	static std::shared_ptr<ViewScenePreview> create(const std::string &pName,
													const std::shared_ptr<IWidgetContext> &pContext);

	[[nodiscard]] IPresenterScenePreview* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterScenePreview* pPresenter) override { presenter = pPresenter; }

	void customRender() override;

	void onUpdate(bool pVisible) override;

	void openView() override;

	void closeView() override;

	void onCursorPosChanged(double pX, double pY) override;

	void onKeyChanged(int pKey, int pScancode, int pAction, int pMods) override;

	void onMouseButton(int pButton, int pAction, int pMods) override;

	void onMouseScroll(double pXOffset, double pYOffset) override;

	[[nodiscard]] const std::shared_ptr<sdk::OrbitCameraExtension> &getCamera() const override { return camera; }

	[[nodiscard]] const glm::vec2 &getMousePos() const override { return mousePos; }

	bool isRotate() override;

	void focusOnThis() override;
};

} // namespace mer::editor::mvp

#endif //VIEWSCENEPREVIEW_H
