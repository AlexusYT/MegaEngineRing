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
// Created by alexus on 27.02.25.
//

#include "ModelScenePreview.h"

#include "KwasarEngine/extensions/MeshExtension.h"
#include "KwasarEngine/gltf/Node.h"
#include "KwasarEngine/render/Renderer.h"
#include "KwasarEngine/scene/Scene3D.h"
#include "mvp/sceneEditor/NodeSelectionHelper.h"

namespace ked {
ModelScenePreview::ModelScenePreview(NodeSelectionHelper* pSelectionHelper)
	: selectionHelper(pSelectionHelper) {
	outlinePass = std::make_shared<ke::RenderPass>();
	selectionHelper->connectOnNodeSelectionChanged([this](const std::vector<ke::Node*> &pNodes, bool pSelected) {
		for (auto node: pNodes) {
			if (node->hasExtension<ke::MeshExtension>()) {
				if (pSelected) {
					scene->getRenderer()->getMainRenderPass()->removeNode(node);
					outlinePass->addNode(node);
				} else {
					outlinePass->removeNode(node);
					scene->getRenderer()->getMainRenderPass()->addNode(node);
				}
			}
		}
	});
}

void ModelScenePreview::setScene(const std::shared_ptr<ke::Scene3D> &pScene) {
	if (pScene == scene) return;
	if (scene) {
		clearSelectedMeshes();
		scene->getRenderer()->removeRenderPass("__editor_outline__");
	}
	if (pScene) pScene->getRenderer()->addRenderPass("__editor_outline__", outlinePass);
	scene = pScene;
	if (presenter) presenter->onSceneChanged();
}

void ModelScenePreview::addSelectedMeshNode(ke::Node* pNode) { selectionHelper->addNode(pNode); }

void ModelScenePreview::clearSelectedMeshes() { selectionHelper->clearSelection(); }
} // namespace ked