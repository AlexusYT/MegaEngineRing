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

#include "EngineSDK/gltf/MeshInstance.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <sigc++/scoped_connection.h>

#include "EngineSDK/bounding/DebugGeometry.h"
#include "EngineSDK/bounding/VolumeAabb.h"
#include "EngineSDK/gltf/Accessor.h"
#include "EngineSDK/gltf/Mesh.h"
#include "EngineSDK/gltf/Primitive.h"
#include "EngineSDK/utils/Transformation.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk {
Node::Node(const Microsoft::glTF::Node &pNode) : nodeAabb(nullptr, "AABB"), geometryAabb(nullptr, "Geometry AABB") {
	name = pNode.name;
	nodeAabb = VolumeAabb::create();
	geometryAabb = VolumeAabb::create();
	debugGeometry = std::make_shared<DebugGeometry>();
	debugGeometry->setIndices({0, 1, 1, 2, 2, 0});
	connection = geometryAabb.connectEvent([this](const std::shared_ptr<VolumeAabb> & /*pAabb*/) { updateNodeAabb(); });
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
}

Node::Node(const std::string &pName) : nodeAabb(nullptr, "AABB"), geometryAabb(nullptr, "Geometry AABB") {

	name = pName;
	nodeAabb = VolumeAabb::create();
	geometryAabb = VolumeAabb::create();
	debugGeometry = std::make_shared<DebugGeometry>();
	debugGeometry->setIndices({0, 1, 1, 2, 2, 0});
	connection = geometryAabb.connectEvent([this](const std::shared_ptr<VolumeAabb> & /*pAabb*/) { updateNodeAabb(); });
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
	pChild->parentNode = nullptr;
	pChild->setTransformParent(nullptr);
	updateNodeAabb();
}

void Node::listGeometryIntersectsAabb(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
									  std::vector<std::pair<glm::vec3, Node*>> &pCandidates) {

	glm::vec3 coord;
	if (!nodeAabb->isIntersects(pRayOrigin, pRayDir, coord)) return;
	for (auto child: getChildren()) {
		//
		child->listGeometryIntersectsAabb(pRayOrigin, pRayDir, pCandidates);
	}
}

void Node::onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) {
	for (auto child: children) {
		auto &global = child->getGlobalTransform();
		auto &local = child->getLocalTransform();
		global->setFromMatrix(pTransformation->getModelMatrix() * local->getModelMatrix());
	}
	Transformable::onGlobalTransformChanged(pTransformation);
}

void Node::updateNodeAabb() {

	glm::vec3 min;
	glm::vec3 max;
	if (geometryAabb.getValue()) { geometryAabb->getBounds(min, max); }
	for (auto child: children) {
		glm::vec3 tmpMin;
		glm::vec3 tmpMax;
		child->getNodeAabb()->getBounds(tmpMin, tmpMax);
		min = glm::min(min, tmpMin);
		max = glm::max(max, tmpMax);
	}
	nodeAabb->setBounds(min, max);
}

MeshInstance::MeshInstance(const Microsoft::glTF::Node &pNode, const std::shared_ptr<Mesh> &pMesh)
	: Node(pNode), mesh(pMesh), data(nullptr, "") {
	mesh->getAabb().connectEvent(
		[this](const std::shared_ptr<VolumeAabb> &pValue) { updateGeometryAabb(pValue, getGlobalTransform()); });
	MeshInstance::onGlobalTransformChanged(getGlobalTransform());
}

MeshInstance::MeshInstance(const std::string &pName, const std::shared_ptr<Mesh> &pMesh)
	: Node(pName), mesh(pMesh), data(nullptr, "") {
	mesh->getAabb().connectEvent(
		[this](const std::shared_ptr<VolumeAabb> &pValue) { updateGeometryAabb(pValue, getGlobalTransform()); });
	MeshInstance::onGlobalTransformChanged(getGlobalTransform());
}

void MeshInstance::listGeometryIntersectsAabb(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
											  std::vector<std::pair<glm::vec3, Node*>> &pCandidates) {
	glm::vec3 coord;
	if (geometryAabb->isIntersects(pRayOrigin, pRayDir, coord)) {
		pCandidates.emplace_back(std::make_pair(coord, this));
	}
	Node::listGeometryIntersectsAabb(pRayOrigin, pRayDir, pCandidates);
}

#define EPSILON 0.000001f
#define CROSS(dest, v1, v2)                                                                                            \
	dest[0] = v1[1] * v2[2] - v1[2] * v2[1];                                                                           \
	dest[1] = v1[2] * v2[0] - v1[0] * v2[2];                                                                           \
	dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
#define DOT(v1, v2) (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2])
#define SUB(dest, v1, v2)                                                                                              \
	dest[0] = v1[0] - v2[0];                                                                                           \
	dest[1] = v1[1] - v2[1];                                                                                           \
	dest[2] = v1[2] - v2[2];

int intersect_triangle(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, const glm::vec3 &pVert0,
					   const glm::vec3 &pVert1, const glm::vec3 &pVert2, glm::vec2 &pCoordOut, float &pDistance) {
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;

	/* find vectors for two edges sharing vert0 */
	SUB(edge1, pVert1, pVert0);
	SUB(edge2, pVert2, pVert0);

	/* begin calculating determinant - also used to calculate U parameter */
	CROSS(pvec, pRayDir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = DOT(edge1, pvec);

	if (det > -EPSILON && det < EPSILON) return 0;
	inv_det = 1.0f / det;

	/* calculate distance from vert0 to ray origin */
	SUB(tvec, pRayOrigin, pVert0);

	/* calculate U parameter and test bounds */
	pCoordOut.x = DOT(tvec, pvec) * inv_det;
	if (pCoordOut.x < 0.0f || pCoordOut.x > 1.0f) return 0;

	/* prepare to test V parameter */
	CROSS(qvec, tvec, edge1);

	/* calculate V parameter and test bounds */
	pCoordOut.y = DOT(pRayDir, qvec) * inv_det;
	if (pCoordOut.y < 0.0f || pCoordOut.x + pCoordOut.y > 1.0f) return 0;

	/* calculate t, ray intersects triangle */
	pDistance = DOT(edge2, qvec) * inv_det;

	return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is at the end in the code                    */
int intersect_triangle1(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, const glm::vec3 &pVert0,
						const glm::vec3 &pVert1, const glm::vec3 &pVert2, glm::vec2 &pCoordOut, float &pDistance) {
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;

	/* find vectors for two edges sharing vert0 */
	SUB(edge1, pVert1, pVert0);
	SUB(edge2, pVert2, pVert0);

	/* begin calculating determinant - also used to calculate U parameter */
	CROSS(pvec, pRayDir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = DOT(edge1, pvec);

	if (det > EPSILON) {
		/* calculate distance from vert0 to ray origin */
		SUB(tvec, pRayOrigin, pVert0);

		/* calculate U parameter and test bounds */
		pCoordOut.x = DOT(tvec, pvec);
		if (pCoordOut.x < 0.0f || pCoordOut.x > det) return 0;

		/* prepare to test V parameter */
		CROSS(qvec, tvec, edge1);

		/* calculate V parameter and test bounds */
		pCoordOut.y = DOT(pRayDir, qvec);
		if (pCoordOut.y < 0.0f || pCoordOut.x + pCoordOut.y > det) return 0;

	} else if (det < -EPSILON) {
		/* calculate distance from vert0 to ray origin */
		SUB(tvec, pRayOrigin, pVert0);

		/* calculate U parameter and test bounds */
		pCoordOut.x = DOT(tvec, pvec);
		/*      printf("*u=%f\n",(float)*u); */
		/*      printf("det=%f\n",det); */
		if (pCoordOut.x > 0.0f || pCoordOut.x < det) return 0;

		/* prepare to test V parameter */
		CROSS(qvec, tvec, edge1);

		/* calculate V parameter and test bounds */
		pCoordOut.x = DOT(pRayDir, qvec);
		if (pCoordOut.x > 0.0f || pCoordOut.x + pCoordOut.x < det) return 0;
	} else
		return 0; /* ray is parallell to the plane of the triangle */


	inv_det = 1.0f / det;

	/* calculate t, ray intersects triangle */
	pDistance = DOT(edge2, qvec) * inv_det;
	pCoordOut.x *= inv_det;
	pCoordOut.y *= inv_det;

	return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
int intersect_triangle2(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, const glm::vec3 &pVert0,
						const glm::vec3 &pVert1, const glm::vec3 &pVert2, glm::vec2 &pCoordOut, float &pDistance) {
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;

	/* find vectors for two edges sharing vert0 */
	SUB(edge1, pVert1, pVert0);
	SUB(edge2, pVert2, pVert0);

	/* begin calculating determinant - also used to calculate U parameter */
	CROSS(pvec, pRayDir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = DOT(edge1, pvec);

	/* calculate distance from vert0 to ray origin */
	SUB(tvec, pRayOrigin, pVert0);
	inv_det = 1.0f / det;

	if (det > EPSILON) {
		/* calculate U parameter and test bounds */
		pCoordOut.x = DOT(tvec, pvec);
		if (pCoordOut.x < 0.0f || pCoordOut.x > det) return 0;

		/* prepare to test V parameter */
		CROSS(qvec, tvec, edge1);

		/* calculate V parameter and test bounds */
		pCoordOut.y = DOT(pRayDir, qvec);
		if (pCoordOut.y < 0.0f || pCoordOut.x + pCoordOut.y > det) return 0;

	} else if (det < -EPSILON) {
		/* calculate U parameter and test bounds */
		pCoordOut.x = DOT(tvec, pvec);
		if (pCoordOut.x > 0.0f || pCoordOut.x < det) return 0;

		/* prepare to test V parameter */
		CROSS(qvec, tvec, edge1);

		/* calculate V parameter and test bounds */
		pCoordOut.y = DOT(pRayDir, qvec);
		if (pCoordOut.y > 0.0f || pCoordOut.x + pCoordOut.y < det) return 0;
	} else
		return 0; /* ray is parallell to the plane of the triangle */

	/* calculate t, ray intersects triangle */
	pDistance = DOT(edge2, qvec) * inv_det;
	(pCoordOut.x) *= inv_det;
	(pCoordOut.y) *= inv_det;

	return 1;
}

/* code rewritten to do tests on the sign of the determinant */
/* the division is before the test of the sign of the det    */
/* and one CROSS has been moved out from the if-else if-else */
int intersect_triangle3(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, const glm::vec3 &pVert0,
						const glm::vec3 &pVert1, const glm::vec3 &pVert2, glm::vec2 &pCoordOut, float &pDistance) {
	float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
	float det, inv_det;

	/* find vectors for two edges sharing vert0 */
	SUB(edge1, pVert1, pVert0);
	SUB(edge2, pVert2, pVert0);

	/* begin calculating determinant - also used to calculate U parameter */
	CROSS(pvec, pRayDir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = DOT(edge1, pvec);

	/* calculate distance from vert0 to ray origin */
	SUB(tvec, pRayOrigin, pVert0);
	inv_det = 1.0f / det;

	CROSS(qvec, tvec, edge1);

	if (det > EPSILON) {
		pCoordOut.x = DOT(tvec, pvec);
		if (pCoordOut.x < 0.0f || pCoordOut.x > det) return 0;

		/* calculate V parameter and test bounds */
		pCoordOut.y = DOT(pRayDir, qvec);
		if (pCoordOut.y < 0.0f || pCoordOut.x + pCoordOut.y > det) return 0;

	} else if (det < -EPSILON) {
		/* calculate U parameter and test bounds */
		pCoordOut.x = DOT(tvec, pvec);
		if (pCoordOut.x > 0.0f || pCoordOut.x < det) return 0;

		/* calculate V parameter and test bounds */
		pCoordOut.y = DOT(pRayDir, qvec);
		if (pCoordOut.y > 0.0f || pCoordOut.x + pCoordOut.y < det) return 0;
	} else
		return 0; /* ray is parallell to the plane of the triangle */

	pDistance = DOT(edge2, qvec) * inv_det;
	(pCoordOut.x) *= inv_det;
	(pCoordOut.y) *= inv_det;

	return 1;
}

bool MeshInstance::isGeometryIntersects(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, glm::vec2 &pIntersectsAt,
										float &pIntersectDistance) const {
	float minDistance = std::numeric_limits<float>::max();
	float distanceTmp;
	glm::vec2 minIntersectedAt{};
	glm::vec2 intersectedAtTmp;
	bool found = false;

	auto modelMatrix = getGlobalTransform()->getModelMatrix();
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
			if (intersect_triangle(pRayOrigin, pRayDir, firstVert, secondVert, thirdVert, intersectedAtTmp,
								   distanceTmp)) {
				debugGeometry->setVertices({firstVert.x, firstVert.y, firstVert.z, secondVert.x, secondVert.y,
											secondVert.z, thirdVert.x, thirdVert.y, thirdVert.z});
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

void MeshInstance::onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) {
	data->modelMat = pTransformation->getModelMatrix();
	data->normalMat = glm::transpose(glm::inverse(glm::mat3(data->modelMat)));
	data.notifyChanged();
	onDataChanged(this);
	updateGeometryAabb(geometryAabb.getValue(), getGlobalTransform());
}

void MeshInstance::onLocalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) {
	Node::onLocalTransformChanged(pTransformation);
}

void MeshInstance::updateGeometryAabb(const std::shared_ptr<VolumeAabb> & /*pAabb*/,
									  const std::shared_ptr<Transformation> &pTransformation) {
	// AABBlocal : the AABB of the object in local space.
	// halfsize, centre, dir[3] : intermediate calculations (Basically an OBB).
	// AABBworld : the AABB of the object in world space (after transformation).

	glm::vec3 min, max;
	mesh->getAabb()->getBounds(min, max);

	auto center{(max + min) * 0.5f};
	auto extents{(max - min) * 0.5f};
	auto modelMatrix = pTransformation->getModelMatrix();
	const glm::vec3 globalCenter{modelMatrix * glm::vec4(center, 1.f)};

	// Scaled orientation
	const glm::vec3 right = modelMatrix[0] * extents.x;
	const glm::vec3 up = modelMatrix[1] * extents.y;
	const glm::vec3 forward = -modelMatrix[2] * extents.z;

	const float newIi = std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, right)) +
						std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, up)) +
						std::abs(glm::dot(glm::vec3{1.f, 0.f, 0.f}, forward));

	const float newIj = std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, right)) +
						std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, up)) +
						std::abs(glm::dot(glm::vec3{0.f, 1.f, 0.f}, forward));

	const float newIk = std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, right)) +
						std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, up)) +
						std::abs(glm::dot(glm::vec3{0.f, 0.f, 1.f}, forward));

	//We not need to divise scale because it's based on the half extention of the AABB
	//const AABB globalAABB(globalCenter, newIi, newIj, newIk);
	glm::vec3 newMin = globalCenter - glm::vec3(newIi, newIj, newIk);
	glm::vec3 newMax = globalCenter + glm::vec3(newIi, newIj, newIk);
	/*glm::vec3 halfsize = (max - min) * 0.5f;
	glm::vec3 offset = (max + min) * 0.5f;
	Vector centre = object.position + offset * object.orientation;
	Vector dir[3];
	dir[0] = object.orientation.column(0); // or is it rows?
	dir[1] = object.orientation.column(1); // or is it rows?
	dir[2] = object.orientation.column(2); // or is it rows?
	float rx = fabs(dir[0].x) * halfsize.x + fabs(dir[1].x) * halfsize.y + fabs(dir[2].x) * halfsize.z;
	float ry = fabs(dir[0].y) * halfsize.x + fabs(dir[1].y) * halfsize.y + fabs(dir[2].y) * halfsize.z;
	float rx = fabs(dir[0].z) * halfsize.x + fabs(dir[1].y) * halfsize.y + fabs(dir[2].z) * halfsize.z;
	AABBworld.min = centre - Vector(rx, ry, rz);
	AABBworld.max = centre + Vector(rx, ry, rz);
	glm::vec3 centre = pTransformation->getModelMatrix() * glm::vec4(offset, 0.f);
	glm::vec3 axis0 = pTransformation->getModelMatrix()[0]; // or is it rows?
	glm::vec3 axis1 = pTransformation->getModelMatrix()[1]; // or is it rows?
	glm::vec3 axis2 = pTransformation->getModelMatrix()[2]; // or is it rows?
	float rx = std::abs(axis0.x) * halfsize.x + std::abs(axis1.x) * halfsize.y + std::abs(axis2.x) * halfsize.z;
	float ry = std::abs(axis0.y) * halfsize.x + std::abs(axis1.y) * halfsize.y + std::abs(axis2.y) * halfsize.z;
	float rz = std::abs(axis0.z) * halfsize.x + std::abs(axis1.y) * halfsize.y + std::abs(axis2.z) * halfsize.z;
	glm::vec3 newMin = centre - glm::vec3(rx, ry, rz);
	glm::vec3 newMax = centre + glm::vec3(rx, ry, rz);*/
	//auto matrix = pTransformation->getModelMatrix();
	//geometryAabb->setBounds(matrix * glm::vec4(min, 1.0f), matrix * glm::vec4(max, 1.0f));
	geometryAabb->setBounds(newMin, newMax);
}

} // namespace mer::sdk