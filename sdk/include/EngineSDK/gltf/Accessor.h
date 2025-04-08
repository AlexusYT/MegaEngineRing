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

#ifndef ACCESSOR_H
#define ACCESSOR_H
#include <glm/vec4.hpp>

#include "EngineSDK/render/Initializable.h"
#include "GLTFSDK/GLTF.h"

namespace mer::sdk {
class BufferView;
}

namespace Microsoft::glTF {
class GLTFResourceReader;
class Document;
struct Accessor;
} // namespace Microsoft::glTF

namespace mer::sdk {

class Accessor {
	Microsoft::glTF::ComponentType componentType = Microsoft::glTF::COMPONENT_UNKNOWN;
	Microsoft::glTF::AccessorType type = Microsoft::glTF::TYPE_UNKNOWN;
	glm::vec3 minSize{};
	glm::vec3 maxSize{};
	std::vector<float> min{};
	std::vector<float> max{};
	std::vector<float> floatData;
	std::vector<uint32_t> uint32Data;
	std::vector<uint16_t> uint16Data;
	std::vector<int16_t> int16Data;
	std::vector<uint8_t> uint8Data;
	std::vector<int8_t> int8Data;
	bool normalized = false;
	size_t count = 0U;

	explicit Accessor(const Microsoft::glTF::Accessor &pGltf, const Microsoft::glTF::Document &pDocument,
					  const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pResourceReader);

	Accessor();

public:
	static std::shared_ptr<Accessor> create(
		const Microsoft::glTF::Accessor &pGltf, const Microsoft::glTF::Document &pDocument,
		const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pResourceReader);

	static std::shared_ptr<Accessor> createVertexAccessor(
		const std::vector<float> &pData, const Microsoft::glTF::AccessorType &pType, const bool pNormalized = false) {
		auto accessor = std::shared_ptr<Accessor>(new Accessor());
		accessor->count = pData.size();
		accessor->normalized = pNormalized;
		accessor->componentType = Microsoft::glTF::COMPONENT_FLOAT;
		accessor->floatData.assign(pData.begin(), pData.end());
		accessor->type = pType;
		accessor->recalcMinMaxSize();
		return accessor;
	}

	template<typename T>
	static std::shared_ptr<Accessor> createIndexAccessor(const std::vector<T> &pData, const bool pNormalized = false) {
		auto accessor = std::shared_ptr<Accessor>(new Accessor());
		accessor->type = Microsoft::glTF::TYPE_SCALAR;
		accessor->count = pData.size();
		accessor->normalized = pNormalized;
		if constexpr (std::is_same_v<T, uint8_t>) {
			accessor->uint8Data.assign(pData.begin(), pData.end());
			accessor->componentType = Microsoft::glTF::COMPONENT_UNSIGNED_BYTE;
		} else if constexpr (std::is_same_v<T, int8_t>) {
			accessor->int8Data.assign(pData.begin(), pData.end());
			accessor->componentType = Microsoft::glTF::COMPONENT_BYTE;
		} else if constexpr (std::is_same_v<T, uint16_t>) {
			accessor->uint16Data.assign(pData.begin(), pData.end());
			accessor->componentType = Microsoft::glTF::COMPONENT_UNSIGNED_SHORT;
		} else if constexpr (std::is_same_v<T, int16_t>) {
			accessor->int16Data.assign(pData.begin(), pData.end());
			accessor->componentType = Microsoft::glTF::COMPONENT_SHORT;
		} else if constexpr (std::is_same_v<T, uint32_t>) {
			accessor->uint32Data.assign(pData.begin(), pData.end());
			accessor->componentType = Microsoft::glTF::COMPONENT_UNSIGNED_INT;
		}
		return accessor;
	}

	[[nodiscard]] const glm::vec3 &getMinSize() const { return minSize; }

	[[nodiscard]] const glm::vec3 &getMaxSize() const { return maxSize; }

	[[nodiscard]] Microsoft::glTF::ComponentType getComponentType() const { return componentType; }

	[[nodiscard]] const void* getData() const;

	[[nodiscard]] uint8_t getComponentTypeSize() const {
		switch (componentType) {
			case Microsoft::glTF::COMPONENT_BYTE:
			case Microsoft::glTF::COMPONENT_UNSIGNED_BYTE: return 1;
			case Microsoft::glTF::COMPONENT_SHORT:
			case Microsoft::glTF::COMPONENT_UNSIGNED_SHORT: return 2;
			case Microsoft::glTF::COMPONENT_UNSIGNED_INT:
			case Microsoft::glTF::COMPONENT_FLOAT: return 4;
			default: return 0;
		}
	}

	[[nodiscard]] uint8_t getTypeCount() const {
		switch (type) {
			case Microsoft::glTF::TYPE_SCALAR: return 1;
			case Microsoft::glTF::TYPE_VEC2: return 2;
			case Microsoft::glTF::TYPE_VEC3: return 3;
			case Microsoft::glTF::TYPE_VEC4:
			case Microsoft::glTF::TYPE_MAT2: return 4;
			case Microsoft::glTF::TYPE_MAT3: return 9;
			case Microsoft::glTF::TYPE_MAT4: return 16;
			default: return 0;
		}
	}

	size_t getByteLength() const { return count * getComponentTypeSize() * getTypeCount(); }

	[[nodiscard]] size_t getCount() const { return count; }

	[[nodiscard]] const std::vector<float> &getFloatData() const { return floatData; }

	[[nodiscard]] const std::vector<uint32_t> &getUint32Data() const { return uint32Data; }

	[[nodiscard]] const std::vector<uint16_t> &getUint16Data() const { return uint16Data; }

	[[nodiscard]] const std::vector<int16_t> &getInt16Data() const { return int16Data; }

	[[nodiscard]] const std::vector<uint8_t> &getUint8Data() const { return uint8Data; }

	[[nodiscard]] const std::vector<int8_t> &getInt8Data() const { return int8Data; }

protected:

	void recalcMinMaxSize();
};

} // namespace mer::sdk

#endif //ACCESSOR_H
