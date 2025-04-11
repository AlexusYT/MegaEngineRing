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

#ifndef MESHINSTANCE_H
#define MESHINSTANCE_H
#include <glm/mat4x4.hpp>
#include <sigc++/scoped_connection.h>

#include "EngineSDK/bounding/VolumeAabb.h"
#include "EngineSDK/extensions/cameras/CameraExtension.h"
#include "EngineSDK/utils/Transformable.h"
#include "EngineUtils/utils/Property.h"
#include "GLTFSDK/GLTF.h"


class DebugGeometry;

namespace mer::sdk {
class VolumeAabb;
}

namespace Microsoft::glTF {
struct Node;
}

namespace mer::sdk {
struct MeshInstanceData {
	glm::mat4 modelMat;
	glm::mat4 normalMat;
};
class Mesh;

class Node : public Transformable {

	Node* parentNode{};
	std::vector<Node*> children;
	std::vector<sigc::scoped_connection> signalConnections;
	sigc::scoped_connection connection;
	std::string name;

protected:
	Property<std::shared_ptr<VolumeAabb>> nodeAabb;
	Property<std::shared_ptr<VolumeAabb>> geometryAabb;
	explicit Node(const Microsoft::glTF::Node &pNode);
	explicit Node(const std::string &pName);

public:
	std::shared_ptr<DebugGeometry> debugGeometry;

	static std::shared_ptr<Node> create(const Microsoft::glTF::Node &pNode) {
		return std::shared_ptr<Node>(new Node(pNode));
	}

	static std::shared_ptr<Node> create(const std::string &pName) { return std::shared_ptr<Node>(new Node(pName)); }

	void addChild(Node* pChild);

	void removeChild(Node* pChild);

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] Node* getParentNode() const { return parentNode; }

	[[nodiscard]] const std::vector<Node*> &getChildren() const { return children; }

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<VolumeAabb>> getNodeAabb() { return nodeAabb.getReadOnly(); }

	virtual void listGeometryIntersectsAabb(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
											std::vector<std::pair<glm::vec3, Node*>> &pCandidates);

protected:
	void onLocalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) override;

	void updateNodeAabb();
};

class MeshInstance : public Node {
	std::shared_ptr<Mesh> mesh;
	Property<MeshInstanceData> data;
	sigc::signal<void(MeshInstance* pSelf)> onDataChanged;

protected:
	MeshInstance(const Microsoft::glTF::Node &pNode, const std::shared_ptr<Mesh> &pMesh);

	MeshInstance(const std::string &pName, const std::shared_ptr<Mesh> &pMesh);

public:
	static std::shared_ptr<MeshInstance> create(const Microsoft::glTF::Node &pNode,
												const std::shared_ptr<Mesh> &pMesh) {
		return std::shared_ptr<MeshInstance>(new MeshInstance(pNode, pMesh));
	}

	static std::shared_ptr<MeshInstance> create(const std::string &pName, const std::shared_ptr<Mesh> &pMesh) {
		return std::shared_ptr<MeshInstance>(new MeshInstance(pName, pMesh));
	}

	[[nodiscard]] PropertyReadOnly<MeshInstanceData> getData() { return data.getReadOnly(); }

	sigc::connection connectDataChanged(const sigc::slot<void(MeshInstance* pSelf)> &pSlot) {
		return onDataChanged.connect(pSlot);
	}

	/**
	 * @brief Return the Axis-Aligned Bounding Box that bounded with the geometry of the mesh that transformed by the
	 * Transformation matrix.
	 * @return Read-only property for VolumeAabb
	 */
	[[nodiscard]] PropertyReadOnly<std::shared_ptr<VolumeAabb>> getGeometryAabb() { return geometryAabb.getReadOnly(); }

	void listGeometryIntersectsAabb(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
									std::vector<std::pair<glm::vec3, Node*>> &pCandidates) override;

	bool isGeometryIntersects(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, glm::vec2 &pIntersectsAt,
							  float &pIntersectDistance) const;

	[[nodiscard]] const std::shared_ptr<Mesh> &getMesh() const { return mesh; }

	void setMesh(const std::shared_ptr<Mesh> &pMesh) { mesh = pMesh; }

protected:
	void onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) override;

	void onLocalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) override;

	void updateGeometryAabb(const std::shared_ptr<VolumeAabb> &pAabb,
							const std::shared_ptr<Transformation> &pTransformation);
};

} // namespace mer::sdk

#endif //MESHINSTANCE_H
