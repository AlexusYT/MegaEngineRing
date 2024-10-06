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
// Created by alexus on 29.09.24.
//

#include "EngineSDK/utils/PropertyJsonSerializers.h"

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "EngineSDK/main/resources/models/Model3DObject.h"
#include "EngineSDK/main/resources/models/Model3DResource.h"
#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::vec<4, float>>::serialize(const glm::vec4 &pValue,
																					  main::Extension* /*pExtension*/) {
	return {pValue.x, pValue.y, pValue.z, pValue.w};
}

glm::vec4 mer::sdk::utils::PropertyJsonSerializer<glm::vec<4, float>>::deserialize(const nlohmann::json &pJson,
																				   main::Extension* /*pExtension*/) {
	return glm::vec4(pJson.at(0), pJson.at(1), pJson.at(2), pJson.at(3));
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::vec<3, float>>::serialize(const glm::vec3 &pValue,
																					  main::Extension* /*pExtension*/) {
	return {pValue.x, pValue.y, pValue.z};
}

glm::vec3 mer::sdk::utils::PropertyJsonSerializer<glm::vec<3, float>>::deserialize(const nlohmann::json &pJson,
																				   main::Extension* /*pExtension*/) {
	return glm::vec3(pJson.at(0), pJson.at(1), pJson.at(2));
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::vec<2, float>>::serialize(const glm::vec2 &pValue,
																					  main::Extension* /*pExtension*/) {
	return {pValue.x, pValue.y};
}

glm::vec2 mer::sdk::utils::PropertyJsonSerializer<glm::vec<2, float>>::deserialize(const nlohmann::json &pJson,
																				   main::Extension* /*pExtension*/) {
	return glm::vec2(pJson.at(0), pJson.at(1));
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::mat<4, 4, float>>::serialize(const glm::mat4 &pValue,
																						 main::Extension* pExtension) {
	nlohmann::json result;
	for (int i = 0; i < 4; i++) {
		result.push_back(PropertyJsonSerializer<glm::mat4::col_type>::serialize(pValue[i], pExtension));
	}
	return result;
}

glm::mat4 mer::sdk::utils::PropertyJsonSerializer<glm::mat<4, 4, float>>::deserialize(const nlohmann::json &pJson,
																					  main::Extension* pExtension) {
	glm::mat4 result;
	for (int i = 0; i < 4; i++) {
		result[i] =
			PropertyJsonSerializer<glm::mat4::col_type>::deserialize(pJson.at(static_cast<size_t>(i)), pExtension);
	}
	return result;
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<std::string>::serialize(const std::string &pValue,
																			   main::Extension* /*pExtension*/) {
	return pValue;
}

std::string mer::sdk::utils::PropertyJsonSerializer<std::string>::deserialize(const nlohmann::json &pJson,
																			  main::Extension* /*pExtension*/) {
	return pJson.get<std::string>();
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<std::shared_ptr<mer::sdk::main::IModel3DObject>>::serialize(
	const std::shared_ptr<main::IModel3DObject> &pValue, main::Extension* /*pExtension*/) {
	if (!pValue) return "null";
	auto resource = pValue->getIModelResource();
	if (!resource) return "null";
	return {{"Resource", resource->asResource()->getResourceUri().string()}, {"Name", pValue->getName()}};
}

std::shared_ptr<mer::sdk::main::IModel3DObject> mer::sdk::utils::PropertyJsonSerializer<
	std::shared_ptr<mer::sdk::main::IModel3DObject>>::deserialize(const nlohmann::json &pJson,
																  const main::Extension* pExtension) {
	if (pJson.is_string() && pJson.get<std::string>() == "null") return nullptr;
	std::string resourceUri = pJson.at("Resource").get<std::string>();
	auto resource = pExtension->loadResourceSync(resourceUri);
	if (auto model = std::dynamic_pointer_cast<main::Model3DResource>(resource)) {
		std::string name = pJson.at("Name").get<std::string>();
		auto obj = model->getModelObject(name);
		if (!obj) return nullptr;
		/*auto shaderResource = pExtension->loadResourceSync("_builtin_/shaders/DefaultInstancedProgram.enshader");
		if (auto shader = std::dynamic_pointer_cast<renderer::ShaderProgram>(shaderResource)) {
			obj->setShader(shader);
		}*/
		return obj;
	}
	return nullptr;
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<std::shared_ptr<mer::sdk::renderer::ShaderProgram>>::serialize(
	const std::shared_ptr<renderer::ShaderProgram> &, main::Extension* /*pExtension*/) {
	return "Shader";
}

std::shared_ptr<mer::sdk::renderer::ShaderProgram> mer::sdk::utils::PropertyJsonSerializer<
	std::shared_ptr<mer::sdk::renderer::ShaderProgram>>::deserialize(const nlohmann::json & /*pJson*/,
																	 main::Extension* /*pExtension*/) {
	return nullptr;
}