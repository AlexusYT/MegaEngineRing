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

#include "EngineSDK/scene/Scene3D.h"

#include "EngineSDK/gltf/Mesh.h"
#include "EngineSDK/render/Renderer.h"

namespace mer::sdk {

Scene3D::Scene3D() : renderer(std::make_shared<Renderer>()) {}

void Scene3D::render() {}

void Scene3D::addRootNode(const std::shared_ptr<Node> &pNewNode) {
	rootNodes.emplace_back(pNewNode.get());

	//if (auto meshInstance = std::dynamic_pointer_cast<MeshInstance>(pNewNode))
	//	renderer->addMeshInstance(meshInstance->getMesh().get(), meshInstance.get());
}

void Scene3D::addNode(const std::shared_ptr<Node> &pParentNode, const std::shared_ptr<Node> &pNode) {
	if (auto meshInstance = std::dynamic_pointer_cast<MeshInstance>(pNode)) { addToMainRenderPass(meshInstance); }
	if (auto lightInstance = std::dynamic_pointer_cast<LightInstance>(pNode)) { addToMainRenderPass(lightInstance); }
	if (!pParentNode) rootNodes.emplace_back(pNode.get());
	else
		pParentNode->addChild(pNode.get());
	nodes.emplace_back(pNode);
	onNodeCollectionChanged();
}

void Scene3D::mergeNodes(const std::vector<std::shared_ptr<Node>> &pNodes) {
	nodes.insert(nodes.cend(), pNodes.begin(), pNodes.end());
	for (auto node: pNodes) {
		if (!node->getParentNode()) rootNodes.emplace_back(node.get());
		if (auto meshInstance = std::dynamic_pointer_cast<MeshInstance>(node)) { addToMainRenderPass(meshInstance); }
		if (auto lightInstance = std::dynamic_pointer_cast<LightInstance>(node)) { addToMainRenderPass(lightInstance); }
	}
	onNodeCollectionChanged();
}

void Scene3D::addMesh(const std::shared_ptr<Mesh> &pMesh) const { renderer->addMesh(pMesh); }

void Scene3D::addMaterial(const std::shared_ptr<Material> &pMaterial) const { renderer->addMaterial(pMaterial); }

void Scene3D::addLightSource(const std::shared_ptr<Light> &pLight) const { renderer->addLightSource(pLight); }

const std::vector<std::shared_ptr<Material>> &Scene3D::getMaterials() const { return renderer->getMaterials(); }

ReportMessagePtr Scene3D::onInitialize() { return Initializable::onInitialize(); }

void Scene3D::onUninitialize() { Initializable::onUninitialize(); }

void Scene3D::addToMainRenderPass(const std::shared_ptr<MeshInstance> &pMeshInstance) const {
	renderer->getMainRenderPass()->addMeshInstance(pMeshInstance->getMesh().get(), pMeshInstance.get());
}

void Scene3D::addToMainRenderPass(const std::shared_ptr<LightInstance> &pLightInstance) const {
	renderer->getMainRenderPass()->addLightInstance(pLightInstance.get());
}
} // namespace mer::sdk