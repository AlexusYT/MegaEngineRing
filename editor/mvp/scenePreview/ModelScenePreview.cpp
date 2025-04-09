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

#include "ModelScenePreview.h"

#include "EngineSDK/gltf/MeshInstance.h"
#include "EngineSDK/render/Renderer.h"
#include "EngineSDK/scene/Scene3D.h"

namespace mer::editor::mvp {
ModelScenePreview::ModelScenePreview() { outlinePass = std::make_shared<sdk::RenderPass>(); }

void ModelScenePreview::setScene(const std::shared_ptr<sdk::Scene3D> &pScene) {
	if (pScene == scene) return;
	if (scene) {
		clearSelectedMeshes();
		scene->getRenderer()->removeRenderPass("__editor_outline__");
	}
	if (pScene) pScene->getRenderer()->addRenderPass("__editor_outline__", outlinePass);
	scene = pScene;
	if (presenter) presenter->onSceneChanged();
}

void ModelScenePreview::addNode(const std::shared_ptr<sdk::Node> &pParentNode,
								const std::shared_ptr<sdk::Node> &pNode) {
	scene->addNode(pParentNode, pNode);
}

void ModelScenePreview::addMaterial(const std::shared_ptr<sdk::Material> &pMaterial) { scene->addMaterial(pMaterial); }

void ModelScenePreview::addSelectedMeshNode(sdk::MeshInstance* pMeshInstance) {
	auto oldMesh = pMeshInstance->getMesh();
	scene->getRenderer()->getMainRenderPass()->removeMeshInstance(oldMesh.get(), pMeshInstance);
	outlinePass->addMeshInstance(oldMesh.get(), pMeshInstance);
	selectedMeshNodes.emplace_back(pMeshInstance);
	/*auto newMesh = sdk::Mesh::create();
	newMesh->setPrimitives(oldMesh->getPrimitives());
	pMeshInstance->changeMesh(newMesh);
	oldMeshes.emplace(newMesh, oldMesh);
	selectedMeshes.emplace_back(newMesh);*/
}

void ModelScenePreview::clearSelectedMeshes() {

	for (auto selectedMeshNode: selectedMeshNodes) {
		auto oldMesh = selectedMeshNode->getMesh();
		outlinePass->removeMeshInstance(oldMesh.get(), selectedMeshNode);
		scene->getRenderer()->getMainRenderPass()->addMeshInstance(oldMesh.get(), selectedMeshNode);
	}
	selectedMeshNodes.clear();
}
} // namespace mer::editor::mvp