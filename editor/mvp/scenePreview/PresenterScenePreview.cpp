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

#include "PresenterScenePreview.h"

#include <memory>

#include "KwasarEngine/bounding/DebugGeometry.h"
#include "KwasarEngine/bounding/VolumeAabb.h"
#include "KwasarEngine/extensions/MeshExtension.h"
#include "KwasarEngine/extensions/cameras/OrbitCameraExtension.h"
#include "KwasarEngine/gltf/Node.h"
#include "KwasarEngine/render/Renderer.h"
#include "KwasarEngine/resources/shaders/builtin/BoundingVolumeProgram.h"
#include "KwasarEngine/resources/shaders/builtin/DefaultProgram.h"
#include "KwasarEngine/scene/Scene3D.h"
#include "KwasarEngine/utils/Transformation.h"
#include "IModelScenePreview.h"
#include "IViewScenePreview.h"

namespace ked {
PresenterScenePreview::PresenterScenePreview(const std::shared_ptr<IViewScenePreview> &pView,
											 const std::shared_ptr<IModelScenePreview> &pModel)
	: view(pView), model(pModel) {
	model->setPresenter(this);
	view->setPresenter(this);
}

void PresenterScenePreview::renderScene() {
	auto &scene = model->getScene();
	if (!scene) return;
	/*if (!scene->isInited()) {
		if (auto msg = scene->initialize()) { ke::Logger::error(msg); }
	}
	*/
	scene->getRenderer()->executeMainPass();
	//for (auto mesh: scene->getMeshes()) mesh->render();
}

void PresenterScenePreview::renderGeometryBoundingVolumes() {
	boundingProgram->use();
	auto &scene = model->getScene();
	if (!scene) return;
	auto camera = view->getCamera();
	auto mousePos = view->getMousePos();
	[[maybe_unused]] glm::vec3 dir =
		camera->propertyTargetPosition.getValue() +
		glm::vec3(glm::inverse(camera->getMatrix()) * glm::vec4(mousePos.x, -mousePos.y, 1.0f, 1.0f)) *
		camera->propertyDistance.getValue();
	glm::vec3 origin = camera->propertyPosition.getValue();
	test->setBounds(dir, origin);
	hoveredMeshNode = nullptr;
	std::vector<std::pair<glm::vec3, ke::Node*>> candidates;
	//test->render();
	for (auto node: scene->getRootNodes()) {
		//
		node->listGeometryIntersectsAabb(origin, dir - origin, candidates);
	} /*
	std::ranges::sort(candidates, [origin](const std::pair<glm::vec3, ke::Node*> &pLhs,
										   const std::pair<glm::vec3, ke::Node*> &pRhs) {
		return glm::distance(pLhs.first, origin) < glm::distance(origin, pRhs.first);
	});*/
	if (candidates.empty()) return;

	float minDistance = std::numeric_limits<float>::max();
	for (auto &[coordinate, node]: candidates) {
		if (auto ext = node->getExtension<ke::MeshExtension>()) {
			glm::vec2 coord;
			float distance;
			if (ext->isGeometryIntersects(origin, glm::normalize(dir - origin), coord, distance)) {
				if (minDistance > distance) {
					minDistance = distance;
					hoveredMeshNode = node;
				}
				//selectedByRay.emplace_back(std::make_pair(distance, meshInst));
			}
		}
	}
	//auto &[coord, meshInst] = selectedByRay.front();
	int i = 0;
	//for (auto &[coord, meshInst]: selectedByRay) {

	//meshInst->debugGeometry->setCube(origin + glm::normalize(dir - origin) * coord);
	/*ke::Logger::out("{}, {}: {}, {}, {}", meshInst->getName(), glm::distance(coord, origin), coord.x, coord.y,
						 coord.z);*/
	if (!hoveredMeshNode) return;
	if (!hoveredMeshNode->debugGeometry->isInited()) hoveredMeshNode->debugGeometry->initialize();
	boundingProgram->setUniform("color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	//hoveredMeshNode->debugGeometry->render();
	auto aabb = hoveredMeshNode->getContentAabb();
	if (!aabb->isInited()) aabb->initialize();
	boundingProgram->setUniform("color", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	aabb->render();

	//if (view->isRotate()) selectedMesh->getLocalTransform()->translate(0.1f, 0, 0);
	//	break;
	i++;
	//}
}

void PresenterScenePreview::renderSelected(bool /*pOutline*/) {
	auto &scene = model->getScene();
	if (!scene) return;
	scene->getRenderer()->executeRenderPass("__editor_outline__");
	/*for (auto selectedMesh: model->getSelectedMeshes()) {
		if (!selectedMesh->isInited()) selectedMesh->initialize();
		selectedMesh->render();
	}
	if (view->isRotate()) {
		for (auto selectedMeshNode: model->getSelectedMeshNodes()) {
			selectedMeshNode->getLocalTransform()->translate(1, 0, 0);
		}
	}*/
	//scene->getMeshes().at(1)->render();
	//scene->render();
}

void PresenterScenePreview::init() {
	boundingProgram = ke::BoundingVolumeProgram::getInstance();
	if (auto msg = boundingProgram->initialize()) { ke::Logger::error(msg); }
	auto defaultProgram = ke::DefaultProgram::getInstance();
	if (!defaultProgram->isInited()) { if (auto msg = defaultProgram->initialize()) { ke::Logger::error(msg); } }
	test = ke::VolumeAabb::create();
	test->initialize();
}

void PresenterScenePreview::onPrimaryMouseKeyPressed() {
	if (applyCurrentAction()) return;
	model->clearSelectedMeshes();
	if (hoveredMeshNode) model->addSelectedMeshNode(hoveredMeshNode);
}

void PresenterScenePreview::onSecondaryMouseKeyPressed() { cancelCurrentAction(); }

void PresenterScenePreview::onSceneChanged() {
	/*auto scene = model->getScene();
	if (!scene) return;*/
	/*std::vector<ke::Node*> nodes;
	for (auto material: scene->getMaterials()) { model->addMaterial(material.get()); }
	for (auto rootNode: scene->getRootNodes()) { getAllNodes(rootNode.get(), nodes); }
	for (auto node: nodes) {
		if (auto instance = dynamic_cast<ke::MeshInstance*>(node)) {
			model->addMeshInstance(instance->getMesh(), instance);
		}
	}*/
	/*for (auto mesh: scene->getMeshes()) {
		//
		mainRenderPass->addMesh(mesh.get());
	}*/
}

void PresenterScenePreview::setFocus() { view->focusOnThis(); }

void PresenterScenePreview::startMovingGesture() {
	if (movingNode) return;
	auto &nodes = model->getSelectedMeshNodes();
	if (nodes.empty()) return;
	for (auto node: nodes) { startPosForNodes.emplace(node, node->getLocalTransform()->getPosition()); }
	movingNode = true;
}

bool PresenterScenePreview::cancelCurrentAction() {
	if (movingNode) {
		for (auto &node: startPosForNodes) { node.first->getLocalTransform()->setPosition(node.second); }

		startPosForNodes.clear();
		movingNode = false;
		return true;
	}
	return false;
}

bool PresenterScenePreview::applyCurrentAction() {
	if (movingNode) {
		startPosForNodes.clear();
		movingNode = false;
		return true;
	}
	return false;
}

bool PresenterScenePreview::onCursorPosChanged(double pX, double pY) {
	const glm::dvec2 pos{pX, pY};
	if (movingNode) {
		auto camera = view->getCamera();
		auto mousePos = view->getMousePos();
		glm::vec3 cameraDirection =
			glm::normalize(camera->propertyPosition.getValue() - camera->propertyTargetPosition.getValue());
		[[maybe_unused]] glm::vec3 dir =
			/*camera->propertyTargetPosition.getValue() +*/
			(cameraDirection +
			 glm::vec3(glm::inverse(camera->getMatrix()) * glm::vec4(mousePos.x, -mousePos.y, 1.0f, 1.0f))) *
			camera->propertyDistance.getValue() * 0.5f;
		if (!lastCursorPosMove) { lastCursorPosMove = pos; }
		/*const glm::vec2 delta = glm::vec2(lastCursorPosMove.value() - pos) * 0.002f * view->getZoom();
		glm::vec3 cameraRight = glm::normalize(glm::cross(camera->propertyUp.getValue(), cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);*/
		for (auto &[node, startPos]: startPosForNodes) {
			auto &globTransform = node->getGlobalTransform();
			node->getLocalTransform()->setPosition(startPos + dir / globTransform->getScale());
		}
		return true;
	}
	if (lastCursorPosMove) lastCursorPosMove.reset();
	return false;
}

void PresenterScenePreview::run() { view->openView(); }

void PresenterScenePreview::stop() { view->closeView(); }
} // namespace ked