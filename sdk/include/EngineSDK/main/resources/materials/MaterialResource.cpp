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
#include <numeric>

#include "EngineSDK/main/resources/ResourceType.h"
#include "EngineSDK/main/resources/textures/ITextureResource.h"
#include "EngineSDK/main/resources/textures/Texture2DImageFormat.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {
MaterialResource::MaterialResource() {
	data.aoMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	data.metallicMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	data.roughnessMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	data.normalMap = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	data.baseColorMap = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

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
	if (auto msg = checkFormat(pBaseColorMap->getFormat(), {Texture2DImageFormat::RGB, Texture2DImageFormat::BGR,
															Texture2DImageFormat::RGBA, Texture2DImageFormat::BGRA})) {
		msg->setTitle("Failed to set base color (albedo) map");
		return msg;
	}
	baseColorMap = pBaseColorMap;
	connectHandlerChanged(pBaseColorMap, &data.baseColorMap);

	return nullptr;
}

std::optional<glm::vec3> MaterialResource::getBaseColor() {
	if (data.baseColorMap.w == 2.0f) return {};
	return {data.baseColorMap};
}

utils::ReportMessagePtr MaterialResource::setNormalMap(const std::shared_ptr<ITextureResource> &pNormalMap) {
	if (!pNormalMap) {
		if (data.normalMap.w == 2.0f) data.normalMap = glm::vec4(1.0f);
		return nullptr;
	}
	if (auto msg = checkFormat(pNormalMap->getFormat(), {Texture2DImageFormat::RGB, Texture2DImageFormat::BGR,
														 Texture2DImageFormat::RGBA, Texture2DImageFormat::BGRA})) {
		msg->setTitle("Failed to set normal map");
		return msg;
	}
	normalMap = pNormalMap;
	connectHandlerChanged(pNormalMap, &data.normalMap);

	return nullptr;
}

std::optional<glm::vec3> MaterialResource::getNormalColor() {
	if (data.normalMap.w == 2.0f) return {};
	return {data.normalMap};
}

utils::ReportMessagePtr MaterialResource::setMetallicMap(const std::shared_ptr<ITextureResource> &pMetallicMap) {

	if (!pMetallicMap) {
		if (data.metallicMap.w == 2.0f) data.metallicMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return nullptr;
	}
	if (auto msg = checkFormat(pMetallicMap->getFormat(), {Texture2DImageFormat::RED})) {
		msg->setTitle("Failed to set metallic map");
		return msg;
	}
	metallicMap = pMetallicMap;
	connectHandlerChanged(pMetallicMap, &data.metallicMap);

	return nullptr;
}

std::optional<float> MaterialResource::getMetallicColor() {
	if (data.metallicMap.w == 2.0f) return {};
	return {data.metallicMap.x};
}

utils::ReportMessagePtr MaterialResource::setRoughnessMap(const std::shared_ptr<ITextureResource> &pRoughnessMap) {
	if (!pRoughnessMap) {
		if (data.roughnessMap.w == 2.0f) data.roughnessMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		return nullptr;
	}
	if (auto msg = checkFormat(pRoughnessMap->getFormat(), {Texture2DImageFormat::RED})) {
		msg->setTitle("Failed to set roughness map");
		return msg;
	}
	roughnessMap = pRoughnessMap;
	connectHandlerChanged(pRoughnessMap, &data.roughnessMap);
	return nullptr;
}

std::optional<float> MaterialResource::getRoughnessColor() {
	if (data.roughnessMap.w == 2.0f) return {};
	return {data.roughnessMap.x};
}

utils::ReportMessagePtr MaterialResource::setAoMap(const std::shared_ptr<ITextureResource> &pAoMap) {
	if (!pAoMap) {
		if (data.aoMap.w == 2.0f) data.aoMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return nullptr;
	}
	if (auto msg = checkFormat(pAoMap->getFormat(), {Texture2DImageFormat::RED})) {
		msg->setTitle("Failed to set ambient occlusion map");
		return msg;
	}
	aoMap = pAoMap;
	connectHandlerChanged(pAoMap, &data.aoMap);
	return nullptr;
}

std::optional<float> MaterialResource::getAoColor() {
	if (data.aoMap.w == 2.0f) return {};
	return {data.aoMap.x};
}

void MaterialResource::connectHandlerChanged(const std::shared_ptr<ITextureResource> &pTextureResource,
											 glm::vec4* pMap) const {
	pTextureResource->getTextureHandle().connectEvent([this, pMap](const uint64_t pHandle) {
		if (pHandle == 0) return;
		*pMap = handleToVec(pHandle);
		onDataChangedSignal();
	});
}

glm::vec4 MaterialResource::handleToVec(const uint64_t pHandle) {
	glm::uvec2 convertedHandle;
	convertedHandle.x = pHandle & 0xFFFFFFFF;
	convertedHandle.y = static_cast<unsigned>(pHandle >> 32) & 0xFFFFFFFF;
	return glm::vec4(uintBitsToFloat(convertedHandle), 0.0f, 2.0f);
}

utils::ReportMessagePtr MaterialResource::checkFormat(const Texture2DImageFormat pFormat,
													  const std::vector<Texture2DImageFormat> &pAcceptableFormats) {
	if (pAcceptableFormats.empty()) return nullptr;
	using namespace std::literals::string_literals;
	for (const auto acceptableFormat: pAcceptableFormats) {
		if (acceptableFormat == pFormat) return nullptr;
	}
	auto msg = utils::ReportMessage::create();
	msg->setMessage("Unsupported texture format for that map type");
	std::string initStr = "Available formats: "s + to_string(*pAcceptableFormats.begin());
	std::string formatsStr = std::accumulate(++pAcceptableFormats.begin(), pAcceptableFormats.end(), std::move(initStr),
											 [](const std::string &pA, auto &pB) { return pA + ", " + to_string(pB); });
	msg->addInfoLine(formatsStr);
	msg->addInfoLine("Trying to set texture with {} format", to_string(pFormat));
	return msg;
}
} // namespace mer::sdk::main