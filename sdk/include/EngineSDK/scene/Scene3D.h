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
// Created by alexus on 24.02.25.
//

#ifndef SCENE3D_H
#define SCENE3D_H
#include "EngineSDK/render/IRenderable.h"
#include "EngineSDK/render/Initializable.h"

namespace mer::sdk {
class MeshInstance;
class Renderer;
class Material;
class Node;
class Mesh;

class Scene3D : public Initializable, public IRenderable {
	std::shared_ptr<Renderer> renderer;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<Node*> rootNodes;

	std::vector<std::shared_ptr<Material>> materials;

protected:
	Scene3D();

public:
	static std::shared_ptr<Scene3D> create() { return std::shared_ptr<Scene3D>(new Scene3D()); }

	void render() override;

	void addRootNode(const std::shared_ptr<Node> &pNewNode);

	void addNode(const std::shared_ptr<Node> &pParentNode, const std::shared_ptr<Node> &pNode);

	void mergeNodes(const std::vector<std::shared_ptr<Node>> &pNodes);

	void addMesh(const std::shared_ptr<Mesh> &pMesh) const;

	void addMaterial(const std::shared_ptr<Material> &pMaterial) const;

	[[nodiscard]] const std::vector<Node*> &getRootNodes() const { return rootNodes; }

	[[nodiscard]] const std::shared_ptr<Renderer> &getRenderer() const { return renderer; }


protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void addToMainRenderPass(const std::shared_ptr<MeshInstance> &pMeshInstance) const;

	void addToMainRenderPassRecursive(const std::shared_ptr<Node> &pNode);
};

} // namespace mer::sdk

#endif //SCENE3D_H
