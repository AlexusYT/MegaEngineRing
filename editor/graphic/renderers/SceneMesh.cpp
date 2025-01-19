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
// Created by alexus on 09.12.23.
//
#if 0

	#include "SceneMesh.h"

	#include <graphic/ProgramWideSettings.h>

	#include "Render/opengl/buffers/VertexArrayObject.h"
	#include "graphic/UIUtils.h"

void SceneMesh::init() { initBuffers(); }

void SceneMesh::update() {
	SceneMeshBase::updateBuffers();

	getRenderBuffer()->drawElementsBind(RenderIndicesBuffer::info.data.size());
}

inline void SceneMesh::getColors(std::vector<glm::vec4> &newColors) { newColors.resize(8); }

inline void SceneMesh::getIndices(std::vector<uint16_t> &newIndices) { UIUtils::getCubeIndices(newIndices); }

inline void SceneMesh::getNormals(std::vector<glm::vec3> &newNormals) { newNormals.resize(8); }

inline void SceneMesh::getUVs(std::vector<glm::vec2> &newUVs) { newUVs.resize(8); }

inline void SceneMesh::getVertices(std::vector<glm::vec3> &newVertices) {
	UIUtils::getCubeVertices(newVertices, {-10, -10, -10}, {10, 10, 10});
}
#endif
