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

#ifndef IMODELSCENEPREVIEW_H
#define IMODELSCENEPREVIEW_H

namespace mer::editor::project {
class Scene3DEditor;
}

namespace mer::sdk {
class Node;
class Renderer;
class Scene3D;
class MeshInstance;
class Mesh;
class RenderPass;
class Material;
} // namespace mer::sdk

namespace mer::editor::mvp {

class IPresenterScenePreview;

class IModelScenePreview {
public:
	virtual ~IModelScenePreview() = default;

	[[nodiscard]] virtual IPresenterScenePreview* getPresenter() const = 0;

	virtual void setPresenter(IPresenterScenePreview* pPresenter) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::Scene3D> &getScene() const = 0;

	virtual void setScene(const std::shared_ptr<sdk::Scene3D> &pScene) = 0;

	[[nodiscard]] virtual const std::shared_ptr<sdk::RenderPass> &getOutlinePass() const = 0;

	[[nodiscard]] virtual const std::vector<sdk::MeshInstance*> &getSelectedMeshNodes() const = 0;

	[[nodiscard]] virtual const std::vector<std::shared_ptr<sdk::Mesh>> &getSelectedMeshes() const = 0;

	virtual void addSelectedMeshNode(sdk::MeshInstance* pMeshInstance) = 0;

	virtual void clearSelectedMeshes() = 0;
};

} // namespace mer::editor::mvp

#endif //IMODELSCENEPREVIEW_H
