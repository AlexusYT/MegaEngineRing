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

#include "EngineSDK/main/resources/models/ModelResource.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::vec<4, float>>::serialize(const glm::vec4 &pValue) {
	return {pValue.x, pValue.y, pValue.z, pValue.w};
}

glm::vec4 mer::sdk::utils::PropertyJsonSerializer<glm::vec<4, float>>::deserialize(const nlohmann::json &pJson) {
	return glm::vec4(pJson.at(0), pJson.at(1), pJson.at(2), pJson.at(3));
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::vec<3, float>>::serialize(const glm::vec3 &pValue) {
	return {pValue.x, pValue.y, pValue.z};
}

glm::vec3 mer::sdk::utils::PropertyJsonSerializer<glm::vec<3, float>>::deserialize(const nlohmann::json &pJson) {
	return glm::vec3(pJson.at(0), pJson.at(1), pJson.at(2));
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::vec<2, float>>::serialize(const glm::vec2 &pValue) {
	return {pValue.x, pValue.y};
}

glm::vec2 mer::sdk::utils::PropertyJsonSerializer<glm::vec<2, float>>::deserialize(const nlohmann::json &pJson) {
	return glm::vec2(pJson.at(0), pJson.at(1));
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<glm::mat<4, 4, float>>::serialize(const glm::mat4 &pValue) {
	nlohmann::json result;
	for (int i = 0; i < 4; i++) { result.push_back(PropertyJsonSerializer<glm::mat4::col_type>::serialize(pValue[i])); }
	return result;
}

glm::mat4 mer::sdk::utils::PropertyJsonSerializer<glm::mat<4, 4, float>>::deserialize(const nlohmann::json &pJson) {
	glm::mat4 result;
	for (int i = 0; i < 4; i++) {
		result[i] = PropertyJsonSerializer<glm::mat4::col_type>::deserialize(pJson.at(static_cast<size_t>(i)));
	}
	return result;
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<std::string>::serialize(const std::string &pValue) {
	return pValue;
}

std::string mer::sdk::utils::PropertyJsonSerializer<std::string>::deserialize(const nlohmann::json &pJson) {
	return pJson.get<std::string>();
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<std::shared_ptr<mer::sdk::main::ModelResource>>::serialize(
	const std::shared_ptr<main::ModelResource> &pValue) {
	return pValue ? pValue->getName() : "null";
}

std::shared_ptr<mer::sdk::main::ModelResource> mer::sdk::utils::PropertyJsonSerializer<
	std::shared_ptr<mer::sdk::main::ModelResource>>::deserialize(const nlohmann::json & /*pJson*/) {
	return nullptr;
}

nlohmann::json mer::sdk::utils::PropertyJsonSerializer<std::shared_ptr<mer::sdk::renderer::ShaderProgram>>::serialize(
	const std::shared_ptr<renderer::ShaderProgram> &) {
	return "Shader";
}

std::shared_ptr<mer::sdk::renderer::ShaderProgram> mer::sdk::utils::PropertyJsonSerializer<
	std::shared_ptr<mer::sdk::renderer::ShaderProgram>>::deserialize(const nlohmann::json & /*pJson*/) {
	return nullptr;
}