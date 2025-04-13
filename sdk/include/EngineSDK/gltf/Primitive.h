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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include <unordered_map>

#include "EngineSDK/bounding/VolumeAabb.h"
#include "EngineSDK/render/Initializable.h"
#include "EngineUtils/utils/Property.h"
#include "GLTFSDK/MicrosoftGeneratorVersion.h"

namespace mer::sdk {}

namespace Microsoft::glTF {
struct MeshPrimitive;
} // namespace Microsoft::glTF

namespace mer::sdk {
class ShaderProgram;
class MaterialSsbo;
class Material;
class Accessor;

enum class MeshMode {
	// No MESH_UNKNOWN because the default of 0 is a valid value in the spec
	// prefix to avoid name conflicts
	MESH_POINTS = 0,
	MESH_LINES = 1,
	MESH_LINE_LOOP = 2,
	MESH_LINE_STRIP = 3,
	MESH_TRIANGLES = 4,
	MESH_TRIANGLE_STRIP = 5,
	MESH_TRIANGLE_FAN = 6
};

class Primitive {
	MeshMode mode;
	std::unordered_map<std::string, std::shared_ptr<Accessor>> accessors;
	std::shared_ptr<Accessor> indexAccessor;
	std::shared_ptr<Accessor> positionAccessor;
	std::shared_ptr<Accessor> uvAccessor;
	std::shared_ptr<Accessor> normalAccessor;
	std::shared_ptr<Accessor> colorAccessor;
	std::shared_ptr<Material> material;
	sigc::signal<void(const std::shared_ptr<Material> &pNewMaterial)> onMaterialChangedSignal;
	Property<std::shared_ptr<VolumeAabb>> aabb;

	explicit Primitive(const MeshMode &pMeshMode, const std::shared_ptr<Accessor> &pPositionAccessor,
					   const std::shared_ptr<Accessor> &pIndexAccessor);

public:
	static std::shared_ptr<Primitive> create(const MeshMode &pMeshMode,
											 const std::shared_ptr<Accessor> &pPositionAccessor,
											 const std::shared_ptr<Accessor> &pIndexAccessor);

	[[nodiscard]] const std::shared_ptr<Material> &getMaterial() const { return material; }

	void setMaterial(const std::shared_ptr<Material> &pMaterial);

	sigc::connection connectOnMaterialChangedSignal(
		const sigc::slot<void(const std::shared_ptr<Material> &pNewMaterial)> &pSlot) {
		pSlot(material);
		return onMaterialChangedSignal.connect(pSlot);
	}

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<VolumeAabb>> getAabb() { return aabb.getReadOnly(); }

	[[nodiscard]] MeshMode getMode() const { return mode; }

	void setMode(MeshMode pMode) { mode = pMode; }

	void setIndexAccessor(const std::shared_ptr<Accessor> &pIndexAccessor) { indexAccessor = pIndexAccessor; }

	void setPositionAccessor(const std::shared_ptr<Accessor> &pPositionAccessor) {
		positionAccessor = pPositionAccessor;
	}

	void setUvAccessor(const std::shared_ptr<Accessor> &pUvAccessor) { uvAccessor = pUvAccessor; }

	void setNormalAccessor(const std::shared_ptr<Accessor> &pNormalAccessor) { normalAccessor = pNormalAccessor; }

	void setColorAccessor(const std::shared_ptr<Accessor> &pColorAccessor) { colorAccessor = pColorAccessor; }

	[[nodiscard]] const std::shared_ptr<Accessor> &getIndexAccessor() const { return indexAccessor; }

	[[nodiscard]] const std::shared_ptr<Accessor> &getPositionAccessor() const { return positionAccessor; }

	[[nodiscard]] const std::shared_ptr<Accessor> &getUvAccessor() const { return uvAccessor; }

	[[nodiscard]] const std::shared_ptr<Accessor> &getNormalAccessor() const { return normalAccessor; }

	[[nodiscard]] const std::shared_ptr<Accessor> &getColorAccessor() const { return colorAccessor; }

	[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Accessor>> &getAccessors() const {
		return accessors;
	}

	void setAccessors(const std::unordered_map<std::string, std::shared_ptr<Accessor>> &pAccessors) {
		accessors = pAccessors;
	}
};

} // namespace mer::sdk

#endif //PRIMITIVE_H
