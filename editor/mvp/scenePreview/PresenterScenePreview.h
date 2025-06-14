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
// Created by alexus on 28.02.25.
//

#ifndef PRESENTERSCENEPREVIEW_H
#define PRESENTERSCENEPREVIEW_H
#include "IPresenterScenePreview.h"

namespace mer::sdk {
class RenderPass;
} // namespace mer::sdk

namespace mer::sdk {
class BoundingVolumeProgram;
class VolumeAabb;
} // namespace mer::sdk

namespace mer::sdk {
class Node;
}

namespace mer::editor::mvp {
class IModelScenePreview;
class IViewScenePreview;

class PresenterScenePreview : public IPresenterScenePreview {
protected:
	std::shared_ptr<IViewScenePreview> view;
	std::shared_ptr<IModelScenePreview> model;
	std::shared_ptr<sdk::VolumeAabb> test;
	sdk::Node* hoveredMeshNode{};
	std::shared_ptr<sdk::BoundingVolumeProgram> boundingProgram{};

	bool movingNode{};
	std::unordered_map<sdk::Node*, glm::vec3> startPosForNodes{};
	std::optional<glm::dvec2> lastCursorPosMove{};

	PresenterScenePreview(const std::shared_ptr<IViewScenePreview> &pView,
						  const std::shared_ptr<IModelScenePreview> &pModel);

public:
	void renderScene() override;

	void renderGeometryBoundingVolumes() override;

	void renderSelected(bool pOutline) override;

	void init() override;

	void onPrimaryMouseKeyPressed() override;

	void onSecondaryMouseKeyPressed() override;

	void onSceneChanged() override;

	void setFocus() override;

	[[nodiscard]] const std::shared_ptr<IModelScenePreview> &getModel() const override { return model; }

	void startMovingGesture() override;

	bool cancelCurrentAction() override;

	bool applyCurrentAction() override;

	bool onCursorPosChanged(double pX, double pY) override;

protected:
	void run() override;

	void stop() override;
};
} // namespace mer::editor::mvp

#endif //PRESENTERSCENEPREVIEW_H
