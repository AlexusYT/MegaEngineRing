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

#ifndef NODE_H
#define NODE_H
#include <glm/mat4x4.hpp>
#include <sigc++/scoped_connection.h>

#include "EngineSDK/bounding/VolumeAabb.h"
#include "EngineSDK/extensions/cameras/CameraExtension.h"
#include "EngineSDK/utils/Transformable.h"
#include "EngineUtils/utils/Property.h"
#include "GLTFSDK/GLTF.h"

namespace mer::sdk {
class Scene3D;
}

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
	int32_t lightDataId{-1};
	int32_t padding0;
	int32_t padding1;
	int32_t padding2;
};

class Mesh;

class Node : public Transformable {
	std::unordered_map<std::type_index, std::shared_ptr<Extension>> extensions;

	Node* parentNode{};
	Scene3D* scene{};
	std::vector<Node*> children;
	std::vector<sigc::scoped_connection> signalConnections;
	sigc::scoped_connection connection;
	std::string name;
	MeshInstanceData data;
	sigc::signal<void(Node* pSelf)> onDataChanged;

	Property<std::shared_ptr<VolumeAabb>> nodeAabb;
	Property<std::shared_ptr<VolumeAabb>> contentAabb;
	std::shared_ptr<VolumeAabb> untransformedContentAabb;

protected:
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

	[[nodiscard]] Scene3D* getScene() const { return scene; }

	void setScene(Scene3D* pScene) { scene = pScene; }

	[[nodiscard]] Node* getParentNode() const { return parentNode; }

	[[nodiscard]] const std::vector<Node*> &getChildren() const { return children; }

	/**
	 * @brief Return the Axis-Aligned Bounding Box that bounded with the geometry of the mesh that transformed by the
	 * Transformation matrix.
	 * If no geometry, then returned AABB is zero: min is (0;0;0), max is (0;0;0)
	 * @return Read-only property for VolumeAabb
	 */
	[[nodiscard]] PropertyReadOnly<std::shared_ptr<VolumeAabb>> getContentAabb() { return contentAabb.getReadOnly(); }

	/**
	 * @brief Return the Axis-Aligned Bounding Box that combined with contentAabb and children nodeAabb.
	 * If no contentAabb and children nodeAabb, then returned AABB is zero: min is (0;0;0), max is (0;0;0)
	 * @return Read-only property for VolumeAabb
	 */
	[[nodiscard]] PropertyReadOnly<std::shared_ptr<VolumeAabb>> getNodeAabb() { return nodeAabb.getReadOnly(); }

	/**
	 * @brief Set light data index. User code should use the LightExtension to set this value. This is an inner method.
	 * @param pLightDataId index in lights vector.
	 */
	void setLightDataId(int32_t pLightDataId) {
		data.lightDataId = pLightDataId;
		onDataChanged.emit(this);
	}

	[[nodiscard]] const MeshInstanceData &getData() const { return data; }

	sigc::connection connectDataChanged(const sigc::slot<void(Node* pSelf)> &pSlot) {
		return onDataChanged.connect(pSlot);
	}

	virtual void listGeometryIntersectsAabb(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir,
											std::vector<std::pair<glm::vec3, Node*>> &pCandidates);

	ReportMessagePtr addExtension(const std::shared_ptr<Extension> &pExtension);

	ReportMessagePtr removeExtension(const std::shared_ptr<Extension> &pExtension) {
		const auto &typeExpr = *pExtension;
		auto &typeInfo = typeid(typeExpr);
		return removeExtension(typeInfo);
	}

	ReportMessagePtr removeExtension(const std::type_index &pType);

	template <typename T>
	bool hasExtension() const { return hasExtension(typeid(T)); }

	bool hasExtension(const std::type_index &pType) const {
		const auto iter = extensions.find(pType);
		return iter != extensions.end();
	}

	template <typename T>
	std::shared_ptr<T> getExtension() const {
		if (const auto it = extensions.find(typeid(T)); it != extensions.end()) {
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr;
	}

	template <typename T>
	std::shared_ptr<T> getExtension() {
		if (const auto it = extensions.find(typeid(T)); it != extensions.end()) {
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr;
	}

	[[nodiscard]] const std::unordered_map<std::type_index, std::shared_ptr<Extension>> &getExtensions() const {
		return extensions;
	}

	void updateContentAabb(const std::shared_ptr<VolumeAabb> &pAabb = nullptr);

	void updateContentAabb(const glm::mat4 &pTransform, const std::shared_ptr<VolumeAabb> &pAabb = nullptr);

protected:
	void onGlobalTransformChanged(const std::shared_ptr<Transformation> &pTransformation) override;

	void updateNodeAabb();
};
} // namespace mer::sdk

#endif //NODE_H
