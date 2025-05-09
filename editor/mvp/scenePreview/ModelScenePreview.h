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

#ifndef MODELSCENEPREVIEW_H
#define MODELSCENEPREVIEW_H
#include <memory>

#include "IModelScenePreview.h"
#include "IPresenterScenePreview.h"
#include "mvp/sceneEditor/NodeSelectionHelper.h"

namespace mer::sdk {
class Renderer;
}

namespace mer::sdk {
class Material;
class RenderPass;
class Mesh;
} // namespace mer::sdk

namespace mer::sdk {
class Scene3D;
}

namespace mer::editor::mvp {

class ModelScenePreview : public IModelScenePreview {

	IPresenterScenePreview* presenter{};
	std::shared_ptr<sdk::Scene3D> scene{};
	std::shared_ptr<sdk::RenderPass> outlinePass;
	NodeSelectionHelper* selectionHelper;

public:
	explicit ModelScenePreview(NodeSelectionHelper* pSelectionHelper);

	[[nodiscard]] IPresenterScenePreview* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterScenePreview* pPresenter) override {
		presenter = pPresenter;
		presenter->onSceneChanged();
	}

	[[nodiscard]] const std::shared_ptr<sdk::Scene3D> &getScene() const override { return scene; }

	void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) override;

	[[nodiscard]] const std::shared_ptr<sdk::RenderPass> &getOutlinePass() const override { return outlinePass; }

	[[nodiscard]] const std::vector<sdk::Node*> &getSelectedMeshNodes() const override {
		return selectionHelper->getSelectedNodes();
	}

	void addSelectedMeshNode(sdk::Node* pNode) override;

	void clearSelectedMeshes() override;
};

} // namespace mer::editor::mvp

#endif //MODELSCENEPREVIEW_H
