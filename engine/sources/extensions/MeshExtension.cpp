//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 29.03.24.
//

#include "KwasarEngine/extensions/MeshExtension.h"

#include <glm/gtx/intersect.hpp>

#include "KwasarEngine/gltf/Accessor.h"
#include "KwasarEngine/gltf/Node.h"
#include "KwasarEngine/gltf/Primitive.h"
#include "KwasarEngine/scene/Scene3D.h"
#include "KwasarEngine/utils/Transformation.h"

namespace ke {
MeshExtension::MeshExtension() : mesh(this, "Mesh") {}

MeshExtension::~MeshExtension() {}

bool MeshExtension::isGeometryIntersects(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
										 glm::vec2 &pIntersectsAt, float &pIntersectDistance) const {
	float minDistance = std::numeric_limits<float>::max();
	float distanceTmp;
	glm::vec2 minIntersectedAt{};
	glm::vec2 intersectedAtTmp;
	bool found = false;

	auto modelMatrix = getNode()->getGlobalTransform()->getModelMatrix();
	for (auto primitive: mesh->getPrimitives()) {
		auto indexAccessor = primitive->getIndexAccessor();
		auto positionAccessor = primitive->getPositionAccessor();
		auto indexData = static_cast<const uint8_t*>(indexAccessor->getData());
		auto posData = static_cast<const glm::vec3*>(positionAccessor->getData());
		auto indexCount = indexAccessor->getComponentTypeSize();
		for (size_t i = 0, maxI = indexAccessor->getCount(); i < maxI; i += 3) {
			size_t firstIndex{};
			memcpy(&firstIndex, &indexData[i * indexCount], indexCount);
			glm::vec3 firstVert = modelMatrix * glm::vec4(posData[firstIndex], 1.0f);
			size_t secondIndex{};
			memcpy(&secondIndex, &indexData[(i + 1) * indexCount], indexCount);
			glm::vec3 secondVert = modelMatrix * glm::vec4(posData[secondIndex], 1.0f);
			size_t thirdIndex{};
			memcpy(&thirdIndex, &indexData[(i + 2) * indexCount], indexCount);
			glm::vec3 thirdVert = modelMatrix * glm::vec4(posData[thirdIndex], 1.0f);
			if (glm::intersectRayTriangle(pRayOrigin, pRayDir, firstVert, secondVert, thirdVert, intersectedAtTmp,
										  distanceTmp)) {
				/*debugGeometry->setVertices({firstVert.x, firstVert.y, firstVert.z, secondVert.x, secondVert.y,
											secondVert.z, thirdVert.x, thirdVert.y, thirdVert.z});*/
				if (minDistance > distanceTmp) {
					minDistance = distanceTmp;
					minIntersectedAt = intersectedAtTmp;
				}
				found = true;
			}
		}
	}
	if (found) {
		pIntersectDistance = minDistance;
		pIntersectsAt = minIntersectedAt;
		return true;
	}
	return false;
}

void MeshExtension::onNodeChanged(Node* pOldNode) {
	if (!getNode()) {
		pOldNode->getScene()->changeMesh(pOldNode, nullptr);
		freeConnectionStorage();
		aabbChangedConnection.disconnect();
		return;
	}
	putConnectionToStorage(mesh.getReturnOverride().connect([this](const std::shared_ptr<Mesh> &pNewMesh) {
		if (auto nodeSelf = getNode()) { nodeSelf->getScene()->changeMesh(nodeSelf, pNewMesh.get()); }
		return pNewMesh;
	}));
	putConnectionToStorage(mesh.connectEvent([this](const std::shared_ptr<Mesh> &pMesh) {
		if (!pMesh) {
			aabbChangedConnection.disconnect();
			return;
		} {}
		aabbChangedConnection = pMesh->getAabb().connectEvent([this](const std::shared_ptr<VolumeAabb> &pValue) {
			if (const auto nodeSelf = getNode()) nodeSelf->updateContentAabb(pValue);
		});
	}));
}
} // namespace ke