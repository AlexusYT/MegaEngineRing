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

#include "KwasarEngine/scene/Scene3D.h"

#include "KwasarEngine/gltf/Mesh.h"
#include "KwasarEngine/render/Renderer.h"

namespace ke {
Scene3D::Scene3D() : renderer(std::make_shared<Renderer>()) {}

void Scene3D::render() {}

void Scene3D::addRootNode(const std::shared_ptr<Node> &pNewNode) {
	rootNodes.emplace_back(pNewNode.get());

	//if (auto meshInstance = std::dynamic_pointer_cast<MeshInstance>(pNewNode))
	//	renderer->addMeshInstance(meshInstance->getMesh().get(), meshInstance.get());
}

void Scene3D::addNode(Node* pParentNode, const std::shared_ptr<Node> &pNode) {
	pNode->setScene(this);
	addToMainRenderPass(pNode);
	if (!pParentNode) rootNodes.emplace_back(pNode.get());
	else
		pParentNode->addChild(pNode.get());
	nodes.emplace_back(pNode);
	onNodeCollectionChanged();
}

void Scene3D::reparentNode(Node* pNode, Node* pNewParent) {
	if (!pNode) return;
	auto curParentNode = pNode->getParentNode();
	//Prevent adding pNode as its parent
	if (pNode == pNewParent) return;

	//User wants to unparent, but it is already a root node
	if (!curParentNode && !pNewParent) { return; }
	//User wants to unparent. Remove the node from the parent node and add it to the root.
	if (curParentNode && !pNewParent) { //curParentNode is non-null and pNewParent is null
		addToRootNodes(pNode);
		curParentNode->removeChild(pNode);
		return;
	}
	//User wants to parent. Add the node to the new parent node and remove it from the root.
	if (!curParentNode && pNewParent) { //curParentNode is null and pNewParent is non-null
		removeFromRootNodes(pNode);
		pNewParent->addChild(pNode);
		return;
	}
	//User wants to reparent. Change the parent.

	curParentNode->removeChild(pNode);
	pNewParent->addChild(pNode);
}

void Scene3D::mergeNodes(const std::vector<std::shared_ptr<Node>> &pNodes) {
	nodes.insert(nodes.cend(), pNodes.begin(), pNodes.end());
	for (auto node: pNodes) {
		node->setScene(this);
		if (!node->getParentNode()) rootNodes.emplace_back(node.get());
		addToMainRenderPass(node);
	}
	onNodeCollectionChanged();
}

void Scene3D::removeNode(Node* pNode) {
	removeNodeImpl(pNode);
	onNodeCollectionChanged();
}

void Scene3D::addMesh(const std::shared_ptr<Mesh> &pMesh) const { renderer->addMesh(pMesh); }

const std::vector<std::shared_ptr<Mesh>> &Scene3D::getMeshes() const { return renderer->getMeshes(); }

void Scene3D::addMaterial(const std::shared_ptr<Material> &pMaterial) const { renderer->addMaterial(pMaterial); }

void Scene3D::addLightSource(const std::shared_ptr<Light> &pLight) const { renderer->addLightSource(pLight); }

const std::vector<std::shared_ptr<Light>> &Scene3D::getLights() const { return renderer->getLights(); }

void Scene3D::changeMesh(Node* pNode, Mesh* pNewMesh) const { renderer->changeMesh(pNode, pNewMesh); }

const std::vector<std::shared_ptr<Material>> &Scene3D::getMaterials() const { return renderer->getMaterials(); }

ReportMessagePtr Scene3D::onInitialize() { return Initializable::onInitialize(); }

void Scene3D::onUninitialize() { Initializable::onUninitialize(); }

void Scene3D::addToMainRenderPass(const std::shared_ptr<Node> &pNode) const {
	renderer->getMainRenderPass()->addNode(pNode.get());
}

void Scene3D::removeNodeImpl(Node* pNode) {
	if (auto parent = pNode->getParentNode(); !parent) removeFromRootNodes(pNode);
	else { parent->removeChild(pNode); }
	if (auto &children = pNode->getChildren(); !children.empty()) { for (auto child: children) removeChildImpl(child); }
	removeChildImpl(pNode);
}

void Scene3D::removeChildImpl(Node* pNode) {
	pNode->setScene(nullptr);
	renderer->removeNode(pNode);
	auto iter =
		std::ranges::find_if(nodes, [pNode](const std::shared_ptr<Node> &pElem) { return pElem.get() == pNode; });

	nodes.erase(iter);
}
} // namespace ke