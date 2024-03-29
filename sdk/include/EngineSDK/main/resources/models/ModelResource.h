// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 26.03.24.
//

#ifndef MODELRESOURCE_H
#define MODELRESOURCE_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "EngineSDK/main/resources/IResource.h"

namespace mer::sdk::main {

class ModelResource : public IResource {

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<uint16_t> indices;
	std::string name;

public:
	ModelResource(std::vector<glm::vec3> pVertices, std::vector<glm::vec2> pUvs, std::vector<glm::vec3> pNormals,
				  std::vector<uint16_t> pIndices, std::string pName)
		: vertices(std::move(pVertices)), uvs(std::move(pUvs)), normals(std::move(pNormals)),
		  indices(std::move(pIndices)), name(std::move(pName)) {}

	[[nodiscard]] const std::vector<glm::vec3> &getVertices() const { return vertices; }

	void setVertices(const std::vector<glm::vec3> &pVertices) { vertices = pVertices; }

	[[nodiscard]] const std::vector<glm::vec2> &getUvs() const { return uvs; }

	void setUvs(const std::vector<glm::vec2> &pUvs) { uvs = pUvs; }

	[[nodiscard]] const std::vector<glm::vec3> &getNormals() const { return normals; }

	void setNormals(const std::vector<glm::vec3> &pNormals) { normals = pNormals; }

	[[nodiscard]] const std::vector<uint16_t> &getIndices() const { return indices; }

	void setIndices(const std::vector<uint16_t> &pIndices) { indices = pIndices; }

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }
};
} // namespace mer::sdk::main
#endif
