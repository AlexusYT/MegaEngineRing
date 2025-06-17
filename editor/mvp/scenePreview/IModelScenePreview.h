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

namespace ked {
class Scene3DEditor;
}

namespace ke {
class Node;
class Renderer;
class Scene3D;
class Mesh;
class RenderPass;
class Material;
} // namespace ke

namespace ked {
class IPresenterScenePreview;

class IModelScenePreview {
public:
	virtual ~IModelScenePreview() = default;

	[[nodiscard]] virtual IPresenterScenePreview* getPresenter() const = 0;

	virtual void setPresenter(IPresenterScenePreview* pPresenter) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ke::Scene3D> &getScene() const = 0;

	virtual void setScene(const std::shared_ptr<ke::Scene3D> &pScene) = 0;

	[[nodiscard]] virtual const std::shared_ptr<ke::RenderPass> &getOutlinePass() const = 0;

	[[nodiscard]] virtual const std::vector<ke::Node*> &getSelectedMeshNodes() const = 0;

	virtual void addSelectedMeshNode(ke::Node* pNode) = 0;

	virtual void clearSelectedMeshes() = 0;
};
} // namespace ked

#endif //IMODELSCENEPREVIEW_H
