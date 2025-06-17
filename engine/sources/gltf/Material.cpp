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
// Created by alexus on 26.02.25.
//

#include "KwasarEngine/gltf/Material.h"

#include <glm/common.hpp>

#include "KwasarEngine/gltf/Texture.h"
#include "GLTFSDK/GLTF.h"

namespace ke {
Material::Material(const Microsoft::glTF::Material &pMaterial,
				   const std::unordered_map<Microsoft::glTF::TextureType, std::shared_ptr<Texture>> &pUsedTextures)
	: usedTextures(pUsedTextures) {
	using namespace Microsoft::glTF;
	name = pMaterial.name;
	alphaMode = static_cast<int>(pMaterial.alphaMode);
	alphaCutoff = pMaterial.alphaCutoff;
	doubleSided = pMaterial.doubleSided;
	data.emissiveFactor.r = pMaterial.emissiveFactor.r;
	data.emissiveFactor.g = pMaterial.emissiveFactor.g;
	data.emissiveFactor.b = pMaterial.emissiveFactor.b;
	data.normalInfo.z = pMaterial.normalTexture.scale;
	data.occlusionInfo.z = pMaterial.occlusionTexture.strength;
	data.metallicRoughnessInfo.z = pMaterial.metallicRoughness.metallicFactor;
	data.metallicRoughnessInfo.w = pMaterial.metallicRoughness.roughnessFactor;
	data.baseColorFactor.r = pMaterial.metallicRoughness.baseColorFactor.r;
	data.baseColorFactor.g = pMaterial.metallicRoughness.baseColorFactor.g;
	data.baseColorFactor.b = pMaterial.metallicRoughness.baseColorFactor.b;
	data.baseColorFactor.a = pMaterial.metallicRoughness.baseColorFactor.a;
}

std::shared_ptr<Material> Material::create(
	const Microsoft::glTF::Material &pMaterial,
	const std::unordered_map<Microsoft::glTF::TextureType, std::shared_ptr<Texture>> &pUsedTextures) {
	return std::shared_ptr < Material > (new Material(pMaterial, pUsedTextures));
}

ReportMessagePtr Material::onInitialize() {
	using namespace Microsoft::glTF;
	if (const auto iter = usedTextures.find(TextureType::BaseColor); iter != usedTextures.end() && iter->second) {
		if (auto msg = iter->second->initialize()) return msg;
		data.baseColorInfo =
			glm::vec4(handleToVec(iter->second->getBindlessHandle()), data.baseColorInfo.z, data.baseColorInfo.w);
	}
	if (const auto iter = usedTextures.find(TextureType::MetallicRoughness);
		iter != usedTextures.end() && iter->second) {
		if (auto msg = iter->second->initialize()) return msg;
		data.metallicRoughnessInfo = glm::vec4(handleToVec(iter->second->getBindlessHandle()),
											   data.metallicRoughnessInfo.z, data.metallicRoughnessInfo.w);
	}
	if (const auto iter = usedTextures.find(TextureType::Emissive); iter != usedTextures.end() && iter->second) {
		if (auto msg = iter->second->initialize()) return msg;
		data.emissiveInfo =
			glm::vec4(handleToVec(iter->second->getBindlessHandle()), data.emissiveInfo.z, data.emissiveInfo.w);
	}

	if (const auto iter = usedTextures.find(TextureType::Occlusion); iter != usedTextures.end() && iter->second) {
		if (auto msg = iter->second->initialize()) return msg;
		data.occlusionInfo =
			glm::vec4(handleToVec(iter->second->getBindlessHandle()), data.occlusionInfo.z, data.occlusionInfo.w);
	}

	if (const auto iter = usedTextures.find(TextureType::Normal); iter != usedTextures.end() && iter->second) {
		if (auto msg = iter->second->initialize()) return msg;
		data.normalInfo =
			glm::vec4(handleToVec(iter->second->getBindlessHandle()), data.normalInfo.z, data.normalInfo.w);
	}
	onMaterialChanged(this);
	return Initializable::onInitialize();
}

void Material::onUninitialize() { Initializable::onUninitialize(); }

glm::vec2 Material::handleToVec(const uint64_t pHandle) {
	glm::uvec2 convertedHandle;
	convertedHandle.x = pHandle & 0xFFFFFFFF;
	convertedHandle.y = static_cast<unsigned>(pHandle >> 32) & 0xFFFFFFFF;
	return glm::uintBitsToFloat(convertedHandle);
}
} // namespace ke