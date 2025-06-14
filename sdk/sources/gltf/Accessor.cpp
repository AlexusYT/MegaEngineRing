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

#include "EngineSDK/gltf/Accessor.h"

#include <epoxy/gl.h>
#include <glm/ext/scalar_common.hpp>

#include "EngineUtils/utils/ReportMessage.h"
#include "GLTFSDK/GLTF.h"
#include "GLTFSDK/GLTFResourceReader.h"

namespace mer::sdk {
Accessor::Accessor(const Microsoft::glTF::Accessor &pGltf, const Microsoft::glTF::Document &pDocument,
				   const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pResourceReader) {
	if (pGltf.min.size() >= 3) {
		minSize.x = pGltf.min[0];
		minSize.y = pGltf.min[1];
		minSize.z = pGltf.min[2];
	}
	min = pGltf.min;
	if (pGltf.max.size() >= 3) {
		maxSize.x = pGltf.max[0];
		maxSize.y = pGltf.max[1];
		maxSize.z = pGltf.max[2];
	}
	max = pGltf.max;
	componentType = pGltf.componentType;
	normalized = pGltf.normalized;
	type = pGltf.type;
	count = pGltf.count;
	switch (componentType) {
		case Microsoft::glTF::COMPONENT_UNKNOWN: break;
		case Microsoft::glTF::COMPONENT_BYTE:
			int8Data = pResourceReader->ReadBinaryData<int8_t>(pDocument, pGltf);
			break;
		case Microsoft::glTF::COMPONENT_UNSIGNED_BYTE:
			uint8Data = pResourceReader->ReadBinaryData<uint8_t>(pDocument, pGltf);
			break;
		case Microsoft::glTF::COMPONENT_SHORT:
			int16Data = pResourceReader->ReadBinaryData<int16_t>(pDocument, pGltf);
			break;
		case Microsoft::glTF::COMPONENT_UNSIGNED_SHORT:
			uint16Data = pResourceReader->ReadBinaryData<uint16_t>(pDocument, pGltf);
			break;
		case Microsoft::glTF::COMPONENT_UNSIGNED_INT:
			uint32Data = pResourceReader->ReadBinaryData<uint32_t>(pDocument, pGltf);
			break;
		case Microsoft::glTF::COMPONENT_FLOAT:
			floatData = pResourceReader->ReadBinaryData<float>(pDocument, pGltf);

			break;
	}
}

Accessor::Accessor() {}

std::shared_ptr<Accessor> Accessor::create(
	const Microsoft::glTF::Accessor &pGltf, const Microsoft::glTF::Document &pDocument,
	const std::shared_ptr<Microsoft::glTF::GLTFResourceReader> &pResourceReader) {
	return std::shared_ptr<Accessor>(new Accessor(pGltf, pDocument, pResourceReader));
}

const void* Accessor::getData() const {
	switch (componentType) {
		case Microsoft::glTF::COMPONENT_BYTE: return int8Data.data();
		case Microsoft::glTF::COMPONENT_UNSIGNED_BYTE: return uint8Data.data();
		case Microsoft::glTF::COMPONENT_SHORT: return int16Data.data();
		case Microsoft::glTF::COMPONENT_UNSIGNED_SHORT: return uint16Data.data();
		case Microsoft::glTF::COMPONENT_UNSIGNED_INT: return uint32Data.data();
		case Microsoft::glTF::COMPONENT_FLOAT: return floatData.data();
		default: ;
	}
	return nullptr;
}

void Accessor::recalcMinMaxSize() {
	if (componentType != Microsoft::glTF::COMPONENT_FLOAT) return;
	if (getTypeCount() != 3) return;

	glm::vec3 minSizeVec(std::numeric_limits<float>::max());
	glm::vec3 maxSizeVec(std::numeric_limits<float>::lowest());
	for (auto iterBegin = floatData.begin(), iterEnd = floatData.end(); iterBegin != iterEnd;) {
		glm::vec3 val;
		val.x = *iterBegin;
		if (++iterBegin == iterEnd) break;
		val.y = *iterBegin;
		if (++iterBegin == iterEnd) break;
		val.z = *iterBegin;
		++iterBegin;
		minSizeVec = glm::min(minSizeVec, val);
		maxSizeVec = glm::max(maxSizeVec, val);
	}

	minSize = minSizeVec;
	maxSize = maxSizeVec;
}
} // namespace mer::sdk
