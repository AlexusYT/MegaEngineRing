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

#ifndef PROPERTYJSONSERIALIZERS_H
#define PROPERTYJSONSERIALIZERS_H

#include <glm/fwd.hpp>

#include "PropertyJsonSerializer.h"

namespace mer::sdk::renderer {
class ShaderProgram;
}

namespace mer::sdk::main {
class ModelResource;
}

namespace mer::sdk::utils {


//
template<>
struct PropertyJsonSerializer<glm::vec4> {
	static nlohmann::json serialize(const glm::vec4 &pValue);
	static glm::vec4 deserialize(const nlohmann::json &pJson);
};

template<>
struct PropertyJsonSerializer<glm::vec3> {
	static nlohmann::json serialize(const glm::vec3 &pValue);
	static glm::vec3 deserialize(const nlohmann::json &pJson);
};

template<>
struct PropertyJsonSerializer<glm::vec2> {
	static nlohmann::json serialize(const glm::vec2 &pValue);
	static glm::vec2 deserialize(const nlohmann::json &pJson);
};

template<>
struct PropertyJsonSerializer<glm::mat4> {
	static nlohmann::json serialize(const glm::mat4 &pValue);
	static glm::mat4 deserialize(const nlohmann::json &pJson);
};

template<>
struct PropertyJsonSerializer<std::string> {
	static nlohmann::json serialize(const std::string &pValue);
	static std::string deserialize(const nlohmann::json &pJson);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<main::ModelResource>> {
	static nlohmann::json serialize(const std::shared_ptr<main::ModelResource> &pValue);
	static std::shared_ptr<main::ModelResource> deserialize(const nlohmann::json &pJson);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<renderer::ShaderProgram>> {
	static nlohmann::json serialize(const std::shared_ptr<renderer::ShaderProgram> &pValue);
	static std::shared_ptr<renderer::ShaderProgram> deserialize(const nlohmann::json &pJson);
};
} // namespace mer::sdk::utils

#endif //PROPERTYJSONSERIALIZERS_H
