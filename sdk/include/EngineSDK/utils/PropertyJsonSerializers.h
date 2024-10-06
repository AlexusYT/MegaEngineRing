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
class IModel3DObject;
} // namespace mer::sdk::main

namespace mer::sdk::utils {


//
template<>
struct PropertyJsonSerializer<glm::vec4> {
	static nlohmann::json serialize(const glm::vec4 &pValue, main::Extension* pExtension);
	static glm::vec4 deserialize(const nlohmann::json &pJson, main::Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<glm::vec3> {
	static nlohmann::json serialize(const glm::vec3 &pValue, main::Extension* pExtension);
	static glm::vec3 deserialize(const nlohmann::json &pJson, main::Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<glm::vec2> {
	static nlohmann::json serialize(const glm::vec2 &pValue, main::Extension* pExtension);
	static glm::vec2 deserialize(const nlohmann::json &pJson, main::Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<glm::mat4> {
	static nlohmann::json serialize(const glm::mat4 &pValue, main::Extension* pExtension);
	static glm::mat4 deserialize(const nlohmann::json &pJson, main::Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::string> {
	static nlohmann::json serialize(const std::string &pValue, main::Extension* pExtension);
	static std::string deserialize(const nlohmann::json &pJson, main::Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<main::IModel3DObject>> {
	static nlohmann::json serialize(const std::shared_ptr<main::IModel3DObject> &pValue, main::Extension* pExtension);
	static std::shared_ptr<main::IModel3DObject> deserialize(const nlohmann::json &pJson,
															 const main::Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<renderer::ShaderProgram>> {
	static nlohmann::json serialize(const std::shared_ptr<renderer::ShaderProgram> &pValue,
									main::Extension* pExtension);
	static std::shared_ptr<renderer::ShaderProgram> deserialize(const nlohmann::json &pJson,
																main::Extension* pExtension);
};
} // namespace mer::sdk::utils

#endif //PROPERTYJSONSERIALIZERS_H
