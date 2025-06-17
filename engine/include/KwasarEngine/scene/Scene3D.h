//  KwasarEngine is an SDK that can help you speed up game development.
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
#include <sigc++/signal.h>

#include "KwasarEngine/render/IRenderable.h"
#include "KwasarEngine/render/Initializable.h"

namespace ke {
class LightInstance;
class Light;
} // namespace ke

namespace ke {
class Renderer;
class Material;
class Node;
class Mesh;

class Scene3D : public Initializable, public IRenderable {
	std::shared_ptr<Renderer> renderer;
	std::vector<std::shared_ptr<Node>> nodes;
	std::vector<Node*> rootNodes;
	sigc::signal<void()> onNodeCollectionChanged;

protected:
	Scene3D();

public:
	static std::shared_ptr<Scene3D> create() { return std::shared_ptr<Scene3D>(new Scene3D()); }

	void render() override;

	void addRootNode(const std::shared_ptr<Node> &pNewNode);

	void addNode(Node* pParentNode, const std::shared_ptr<Node> &pNode);

	void reparentNode(Node* pNode, Node* pNewParent);

	void mergeNodes(const std::vector<std::shared_ptr<Node>> &pNodes);

	void removeNode(Node* pNode);

	void addMesh(const std::shared_ptr<Mesh> &pMesh) const;

	[[nodiscard]] const std::vector<std::shared_ptr<Mesh>> &getMeshes() const;

	void addMaterial(const std::shared_ptr<Material> &pMaterial) const;

	void addLightSource(const std::shared_ptr<Light> &pLight) const;

	[[nodiscard]] const std::vector<std::shared_ptr<Light>> &getLights() const;

	void changeMesh(Node* pNode, Mesh* pMesh) const;

	[[nodiscard]] const std::vector<std::shared_ptr<Material>> &getMaterials() const;

	[[nodiscard]] const std::vector<std::shared_ptr<Node>> &getNodes() const { return nodes; }

	[[nodiscard]] const std::vector<Node*> &getRootNodes() const { return rootNodes; }

	[[nodiscard]] const std::shared_ptr<Renderer> &getRenderer() const { return renderer; }

	sigc::connection connectOnNodeCollectionChanged(const sigc::slot<void()> &pSlot) {
		return onNodeCollectionChanged.connect(pSlot);
	}

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void addToMainRenderPass(const std::shared_ptr<Node> &pNode) const;

	void removeNodeImpl(Node* pNode);

	void removeChildImpl(Node* pNode);

	void removeFromRootNodes(Node* pNode) { std::erase(rootNodes, pNode); }

	void addToRootNodes(Node* pNode) { rootNodes.emplace_back(pNode); }
};
} // namespace ke

#endif //SCENE3D_H