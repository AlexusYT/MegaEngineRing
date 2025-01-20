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

#ifndef PROPERTYJSONSERIALIZERS_H
#define PROPERTYJSONSERIALIZERS_H

#include <glm/fwd.hpp>

#include "PropertyJsonSerializer.h"

namespace mer::sdk {
class ITextureResource;
class IMaterialResource;
class ShaderProgram;
class IModel3DObject;
} // namespace mer::sdk

namespace mer::sdk {


//
template<>
struct PropertyJsonSerializer<glm::vec4> {
	static nlohmann::json serialize(const glm::vec4 &pValue, Extension* pExtension);
	static glm::vec4 deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<glm::vec3> {
	static nlohmann::json serialize(const glm::vec3 &pValue, Extension* pExtension);
	static glm::vec3 deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<glm::vec2> {
	static nlohmann::json serialize(const glm::vec2 &pValue, Extension* pExtension);
	static glm::vec2 deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<glm::mat4> {
	static nlohmann::json serialize(const glm::mat4 &pValue, Extension* pExtension);
	static glm::mat4 deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::string> {
	static nlohmann::json serialize(const std::string &pValue, Extension* pExtension);
	static std::string deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<IModel3DObject>> {
	static nlohmann::json serialize(const std::shared_ptr<IModel3DObject> &pValue, Extension* pExtension);
	static std::shared_ptr<IModel3DObject> deserialize(const nlohmann::json &pJson,
															 const Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<ShaderProgram>> {
	static nlohmann::json serialize(const std::shared_ptr<ShaderProgram> &pValue, Extension* pExtension);
	static std::shared_ptr<ShaderProgram> deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<ITextureResource>> {
	static nlohmann::json serialize(const std::shared_ptr<ITextureResource> &pValue, Extension* pExtension);
	static std::shared_ptr<ITextureResource> deserialize(const nlohmann::json &pJson,
															   const Extension* pExtension);
};

template<>
struct PropertyJsonSerializer<std::shared_ptr<IMaterialResource>> {
	static nlohmann::json serialize(const std::shared_ptr<IMaterialResource> &pValue,
									Extension* pExtension);
	static std::shared_ptr<IMaterialResource> deserialize(const nlohmann::json &pJson,
																const Extension* pExtension);
};
} // namespace mer::sdk

#endif //PROPERTYJSONSERIALIZERS_H
