//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 29.09.24.
//

#include "EngineSDK/utils/PropertyJsonSerializers.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "EngineSDK/resources/shaders/ShaderProgram.h"
#include "EngineSDK/extensions/Extension.h"
#include "EngineSDK/resources/materials/IMaterialResource.h"
#include "EngineSDK/resources/models/Model3DObject.h"
#include "EngineSDK/resources/models/Model3DResource.h"
#include "EngineSDK/resources/textures/ITextureResource.h"

namespace mer::sdk {
nlohmann::json PropertyJsonSerializer<glm::vec<4, float>>::serialize(const glm::vec4 &pValue,
																	 Extension* /*pExtension*/) {
	return {pValue.x, pValue.y, pValue.z, pValue.w};
}

glm::vec4 PropertyJsonSerializer<glm::vec<4, float>>::deserialize(const nlohmann::json &pJson,
																  Extension* /*pExtension*/) {
	return glm::vec4(pJson.at(0), pJson.at(1), pJson.at(2), pJson.at(3));
}

nlohmann::json PropertyJsonSerializer<glm::vec<3, float>>::serialize(const glm::vec3 &pValue,
																	 Extension* /*pExtension*/) {
	return {pValue.x, pValue.y, pValue.z};
}

glm::vec3 PropertyJsonSerializer<glm::vec<3, float>>::deserialize(const nlohmann::json &pJson,
																  Extension* /*pExtension*/) {
	return glm::vec3(pJson.at(0), pJson.at(1), pJson.at(2));
}

nlohmann::json PropertyJsonSerializer<glm::vec<2, float>>::serialize(const glm::vec2 &pValue,
																	 Extension* /*pExtension*/) {
	return {pValue.x, pValue.y};
}

glm::vec2 PropertyJsonSerializer<glm::vec<2, float>>::deserialize(const nlohmann::json &pJson,
																  Extension* /*pExtension*/) {
	return glm::vec2(pJson.at(0), pJson.at(1));
}

nlohmann::json PropertyJsonSerializer<glm::mat<4, 4, float>>::serialize(const glm::mat4 &pValue,
																		Extension* pExtension) {
	nlohmann::json result;
	for (int i = 0; i < 4; i++) {
		result.push_back(PropertyJsonSerializer<glm::mat4::col_type>::serialize(pValue[i], pExtension));
	}
	return result;
}

glm::mat4 PropertyJsonSerializer<glm::mat<4, 4, float>>::deserialize(const nlohmann::json &pJson,
																	 Extension* pExtension) {
	glm::mat4 result;
	for (int i = 0; i < 4; i++) {
		result[i] =
			PropertyJsonSerializer<glm::mat4::col_type>::deserialize(pJson.at(static_cast<size_t>(i)), pExtension);
	}
	return result;
}

nlohmann::json PropertyJsonSerializer<std::string>::serialize(const std::string &pValue,
															  Extension* /*pExtension*/) {
	return pValue;
}

std::string PropertyJsonSerializer<std::string>::deserialize(const nlohmann::json &pJson,
															 Extension* /*pExtension*/) {
	return pJson.get<std::string>();
}

nlohmann::json PropertyJsonSerializer<std::shared_ptr<IModel3DObject>>::serialize(
	const std::shared_ptr<IModel3DObject> &pValue, Extension* /*pExtension*/) {
	if (!pValue) return "null";
	const auto resource = pValue->getIModelResource();
	if (!resource) return "null";
	return {{"Resource", resource->asResource()->getResourceUri().string()}, {"Name", pValue->getName()}};
}

std::shared_ptr<IModel3DObject> PropertyJsonSerializer<std::shared_ptr<IModel3DObject>>::deserialize(
	const nlohmann::json &pJson, const Extension* pExtension) {
	if (pJson.is_string() && pJson.get<std::string>() == "null") return nullptr;
	const auto resourceUri = pJson.at("Resource").get<std::string>();
	const auto resource = pExtension->loadResourceSync(resourceUri);
	if (const auto model = std::dynamic_pointer_cast<Model3DResource>(resource)) {
		const auto name = pJson.at("Name").get<std::string>();
		return model->getModelObject(name);
	}
	return nullptr;
}

nlohmann::json PropertyJsonSerializer<std::shared_ptr<ShaderProgram>>::serialize(
	const std::shared_ptr<ShaderProgram> &, Extension* /*pExtension*/) {
	return "Shader";
}

std::shared_ptr<ShaderProgram> PropertyJsonSerializer<std::shared_ptr<ShaderProgram>>::deserialize(
	const nlohmann::json & /*pJson*/, Extension* /*pExtension*/) {
	return nullptr;
}

nlohmann::json PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::serialize(
	const std::shared_ptr<ITextureResource> &pValue, Extension* /*pExtension*/) {
	if (!pValue) return "null";
	return pValue->asResource()->getResourceUri().string();
}

std::shared_ptr<ITextureResource> PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::deserialize(
	const nlohmann::json &pJson, const Extension* pExtension) {

	if (pJson.is_string() && pJson.get<std::string>() == "null") return nullptr;
	const auto resourceUri = pJson.get<std::string>();
	const auto resource = pExtension->loadResourceSync(resourceUri);
	return std::dynamic_pointer_cast<ITextureResource>(resource);
}

nlohmann::json PropertyJsonSerializer<std::shared_ptr<IMaterialResource>>::serialize(
	const std::shared_ptr<IMaterialResource> &pValue, Extension* /*pExtension*/) {
	/*nlohmann::json baseColor;
	if (const auto map = pValue->getBaseColorMap()) {
		baseColor = PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::serialize(map, pExtension);
	} else
		baseColor = PropertyJsonSerializer<glm::vec3>::serialize(pValue->getBaseColor().value(), pExtension);
	nlohmann::json normal;
	if (const auto map = pValue->getNormalMap()) {
		normal = PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::serialize(map, pExtension);
	} else
		normal = PropertyJsonSerializer<glm::vec3>::serialize(pValue->getNormalColor().value(), pExtension);
	nlohmann::json metallic;
	if (const auto map = pValue->getMetallicMap()) {
		metallic = PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::serialize(map, pExtension);
	} else
		metallic = PropertyJsonSerializer<float>::serialize(pValue->getMetallicColor().value(), pExtension);
	nlohmann::json roughness;
	if (const auto map = pValue->getRoughnessMap()) {
		roughness = PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::serialize(map, pExtension);
	} else
		roughness = PropertyJsonSerializer<float>::serialize(pValue->getRoughnessColor().value(), pExtension);
	nlohmann::json ao;
	if (const auto map = pValue->getAoMap()) {
		ao = PropertyJsonSerializer<std::shared_ptr<ITextureResource>>::serialize(map, pExtension);
	} else
		ao = PropertyJsonSerializer<float>::serialize(pValue->getAoColor().value(), pExtension);

	return {{"BaseColor", baseColor},
			{"Normal", normal},
			{"Metallic", metallic},
			{"Roughness", roughness},
	{"AmbientOcclusion", ao}};*/
	if (!pValue) return "null";
	return pValue->asResource()->getResourceUri().string();
}

std::shared_ptr<IMaterialResource> PropertyJsonSerializer<std::shared_ptr<IMaterialResource>>::deserialize(
	const nlohmann::json &pJson, const Extension* pExtension) {

	if (pJson.is_string() && pJson.get<std::string>() == "null") return nullptr;
	const auto resourceUri = pJson.get<std::string>();
	const auto resource = pExtension->loadResourceSync(resourceUri);
	return std::dynamic_pointer_cast<IMaterialResource>(resource);
}
} // namespace mer::sdk