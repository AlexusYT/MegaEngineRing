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
// Created by alexus on 25.02.25.
//

#include "EngineSDK/gltf/Node.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <sigc++/scoped_connection.h>

#include "EngineSDK/bounding/DebugGeometry.h"
#include "EngineSDK/bounding/VolumeAabb.h"
#include "EngineSDK/gltf/Accessor.h"
#include "EngineSDK/gltf/Mesh.h"
#include "EngineSDK/gltf/Primitive.h"
#include "EngineSDK/utils/Transformation.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk {
Node::Node(const Microsoft::glTF::Node &pNode)
	: nodeAabb(nullptr, "AABB"), contentAabb(nullptr, "Geometry AABB") {
	name = pNode.name;
	nodeAabb = VolumeAabb::create();
	contentAabb = VolumeAabb::create();
	debugGeometry = std::make_shared<DebugGeometry>();
	debugGeometry->setIndices({0, 1, 1, 2, 2, 0});
	connection = contentAabb.connectEvent([this](const std::shared_ptr<VolumeAabb> & /*pAabb*/) { updateNodeAabb(); });
	switch (pNode.GetTransformationType()) {
		case Microsoft::glTF::TRANSFORMATION_IDENTITY: break;
		case Microsoft::glTF::TRANSFORMATION_MATRIX:
			getLocalTransform()->setFromMatrix(glm::make_mat4(pNode.matrix.values.data()));
			break;
		case Microsoft::glTF::TRANSFORMATION_TRS:
			const auto pos = glm::vec3(pNode.translation.x, pNode.translation.y, pNode.translation.z);
			const auto rot = glm::quat(pNode.rotation.w, pNode.rotation.x, pNode.rotation.y, pNode.rotation.z);
			const auto scale = glm::vec3(pNode.scale.x, pNode.scale.y, pNode.scale.z);
			getLocalTransform()->setFromArgs(pos, rot, scale);
			break;
	}
	Node::onGlobalTransformChanged(getGlobalTransform());
}

Node::Node(const std::string &pName)
	: nodeAabb(nullptr, "AABB"), contentAabb(nullptr, "Geometry AABB") {
	name = pName;
	nodeAabb = VolumeAabb::create();
	contentAabb = VolumeAabb::create();
	debugGeometry = std::make_shared<DebugGeometry>();
	debugGeometry->setIndices({0, 1, 1, 2, 2, 0});
	connection = contentAabb.connectEvent([this](const std::shared_ptr<VolumeAabb> & /*pAabb*/) { updateNodeAabb(); });
	Node::onGlobalTransformChanged(getGlobalTransform());
}

void Node::addChild(Node* pChild) {
	children.emplace_back(pChild);
	pChild->parentNode = this;
	pChild->setTransformParent(this);
	updateNodeAabb();
	signalConnections.emplace_back(pChild->getNodeAabb().connectEvent(
		[this](const std::shared_ptr<VolumeAabb> & /*pAabb*/) { updateNodeAabb(); }));
}

void Node::removeChild(Node* pChild) {
	auto iter = std::ranges::remove(children, pChild).begin();
	auto e = iter - children.begin();
	signalConnections.erase(signalConnections.begin() + e);
	children.erase(iter);
	pChild->parentNode = nullptr;
	pChild->setTransformParent(nullptr);
	updateNodeAabb();
}

void Node::listGeometryIntersectsAabb(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
									  std::vector<std::pair<glm::vec3, Node*>> &pCandidates) {
	glm::vec3 coord;
	if (!nodeAabb->isIntersects(pRayOrigin, pRayDir, coord)) return;
	if (getContentAabb()->isIntersects(pRayOrigin, pRayDir, coord)) {
		pCandidates.emplace_back(std::make_pair(coord, this));
	}
	for (auto child: getChildren()) {
		//
		child->listGeometryIntersectsAabb(pRayOrigin, pRayDir, pCandidates);
	}
}

ReportMessagePtr Node::addExtension(const std::shared_ptr<Extension> &pExtension) {
	const auto &typeExpr = *pExtension; // Workaround for clang -Wpotentially-evaluated-expression
	auto &typeInfo = typeid(typeExpr);

	if (const auto iter = extensions.find(typeInfo); iter != extensions.end()) {
		auto msg = ReportMessage::create();
		msg->setTitle("Unable to add an extension");
		msg->setMessage("Extension with that name already added");
		msg->addInfoLine("Already stored ext typename: {}", Utils::getTypeName(iter->second.get()));
		msg->addInfoLine("New ext typename: {}", Utils::getTypeName(pExtension.get()));
		msg->addInfoLine("Note that single ext instance can be added to single Node instance");
		return msg;
	}
	try { pExtension->setNode(this); }
	catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Unable to add an extension");
		msg->setMessage("Exception occurred in Extension::onNodeChanging()");
		msg->addInfoLine("Extension typename: {}", Utils::getTypeName(pExtension.get()));
		return msg;
	}
	extensions.emplace(typeInfo, pExtension);
	return nullptr;
}

ReportMessagePtr Node::removeExtension(const std::type_index &pType) {
	const auto iter = extensions.find(pType);
	if (iter == extensions.end()) { return nullptr; }
	try { iter->second->setNode(nullptr); }
	catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Unable to remove an extension");
		msg->setMessage("Exception occurred in Extension::onNodeChanging()");
		msg->addInfoLine("Extension typename: {} ({})", iter->second->getTypeName(), Utils::demangle(pType.name()));
		return msg;
	}
	extensions.erase(iter);
	return nullptr;
}

void Node::updateContentAabb(const std::shared_ptr<VolumeAabb> &pAabb) {
	if (pAabb) untransformedContentAabb = pAabb;
	if (!untransformedContentAabb) return;
	glm::vec3 min, max;
	untransformedContentAabb->transform(getGlobalTransform()->getModelMatrix(), min, max);
	contentAabb->setBounds(min, max);
}

void Node::updateContentAabb(const glm::mat4 &pTransform, const std::shared_ptr<VolumeAabb> &pAabb) {
	if (pAabb) untransformedContentAabb = pAabb;
	if (!untransformedContentAabb) return;
	glm::vec3 min, max;
	untransformedContentAabb->transform(pTransform, min, max);
	contentAabb->setBounds(min, max);
}

void Node::onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) {
	auto modelMatrix = pTransformation->getModelMatrix();
	for (auto child: children) {
		auto &global = child->getGlobalTransform();
		auto &local = child->getLocalTransform();
		global->setFromMatrix(modelMatrix * local->getModelMatrix());
	}
	data.modelMat = modelMatrix;
	data.normalMat = glm::transpose(glm::inverse(glm::mat3(data.modelMat)));
	onDataChanged(this);
	updateContentAabb(modelMatrix);
	Transformable::onGlobalTransformChanged(pTransformation);
}

void Node::updateNodeAabb() {
	glm::vec3 min;
	glm::vec3 max;
	if (contentAabb.getValue()) { contentAabb->getBounds(min, max); }
	for (auto child: children) {
		glm::vec3 tmpMin;
		glm::vec3 tmpMax;
		child->getNodeAabb()->getBounds(tmpMin, tmpMax);
		min = glm::min(min, tmpMin);
		max = glm::max(max, tmpMax);
	}
	nodeAabb->setBounds(min, max);
}
} // namespace mer::sdk
