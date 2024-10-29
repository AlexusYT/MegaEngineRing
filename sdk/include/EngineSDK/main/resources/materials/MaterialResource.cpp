//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 22.10.24.
//

#include "MaterialResource.h"

#include <glm/common.hpp>
#include <glm/vec2.hpp>

#include "EngineSDK/main/resources/ResourceType.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"
#include "EngineSDK/main/resources/textures/Texture2DImageFormat.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {
MaterialResource::MaterialResource() {}

std::shared_ptr<MaterialResource> MaterialResource::create() {
	return std::shared_ptr<MaterialResource>(new MaterialResource());
}

ResourceType MaterialResource::getResourceType() { return ResourceType::MATERIAL; }

void MaterialResource::render() {}

utils::ReportMessagePtr MaterialResource::setBaseColorMap(const std::shared_ptr<ITextureResource> &pBaseColorMap) {
	if (!pBaseColorMap) {
		if (data.baseColorMap.w == 2.0f) data.baseColorMap = glm::vec4(1.0f);
		return nullptr;
	}
	switch (auto format = pBaseColorMap->getFormat()) {

		case Texture2DImageFormat::RGB: break;
		case Texture2DImageFormat::BGR: break;
		case Texture2DImageFormat::RGBA: break;
		case Texture2DImageFormat::BGRA: break;
		default:
			auto msg = utils::ReportMessage::create();
			msg->setTitle("Failed to set base color (albedo) map");
			msg->setMessage("Unsupported texture format for that map type. Must be RGB or RGBA");
			msg->addInfoLine("Trying to set texture with {} format", to_string(format));
			return msg;
	}
	auto handle = pBaseColorMap->getTextureHandle() + 1;
	if (handle == 0) {
		auto msg = utils::ReportMessage::create();
		msg->setTitle("Failed to set base color (albedo) map");
		msg->setMessage("OpenGL returned null handle");
		msg->addInfoLine("Is the ARB_bindless_texture extension supported by your videocard?");
		return msg;
	}

	data.baseColorUint = handle;
	data.baseColorMap = handleToVec(handle);
	auto test = glm::floatBitsToUint(glm::vec2(data.baseColorMap.x, data.baseColorMap.y));
	utils::Logger::out("{} {}", test.x, test.y);
	baseColorMap = pBaseColorMap;
	onDataChangedSignal();
	return nullptr;
}

std::optional<glm::vec3> MaterialResource::getBaseColor() {
	if (data.baseColorMap.w == 2.0f) return {};
	return {data.baseColorMap};
}

utils::ReportMessagePtr MaterialResource::setNormalMap(const std::shared_ptr<ITextureResource> &pNormalMap) {
	normalMap = pNormalMap;
	return nullptr;
}

std::optional<glm::vec3> MaterialResource::getNormalColor() {
	if (data.normalMap.w == 2.0f) return {};
	return {data.normalMap};
}

utils::ReportMessagePtr MaterialResource::setMetallicMap(const std::shared_ptr<ITextureResource> &pMetallicMap) {
	metallicMap = pMetallicMap;
	return nullptr;
}

std::optional<float> MaterialResource::getMetallicColor() {
	if (data.metallicMap.w == 2.0f) return {};
	return {data.metallicMap.x};
}

utils::ReportMessagePtr MaterialResource::setRoughnessMap(const std::shared_ptr<ITextureResource> &pRoughnessMap) {
	roughnessMap = pRoughnessMap;
	return nullptr;
}

std::optional<float> MaterialResource::getRoughnessColor() {
	if (data.roughnessMap.w == 2.0f) return {};
	return {data.roughnessMap.x};
}

utils::ReportMessagePtr MaterialResource::setAoMap(const std::shared_ptr<ITextureResource> &pAoMap) {
	aoMap = pAoMap;
	return nullptr;
}

std::optional<float> MaterialResource::getAoColor() {
	if (data.aoMap.w == 2.0f) return {};
	return {data.aoMap.x};
}

glm::vec4 MaterialResource::handleToVec(const uint64_t pHandle) {
	glm::uvec2 convertedHandle;
	convertedHandle.y = pHandle & 0xFFFFFFFF;
	convertedHandle.x = static_cast<unsigned>(pHandle >> 32) & 0xFFFFFFFF;
	return glm::vec4(uintBitsToFloat(convertedHandle), 0.0f, 2.0f);
}
} // namespace mer::sdk::main