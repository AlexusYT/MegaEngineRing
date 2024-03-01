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
// Created by alexus on 09.12.23.
//

#ifndef SCENEMESH_H
#define SCENEMESH_H
#if 0

	#include "SceneMeshBase.h"
	#include "SceneMeshFwd.h"
	#include "graphic/viewport/OpenGL.h"

class SceneMesh : public SceneMeshBase {
	OpenGL* render;

	SceneMesh() = default;

public:
	static SceneMeshUPtr create() { return SceneMeshUPtr(new (std::nothrow) SceneMesh()); }

	void init();

	void update();

protected:
	void getColors(std::vector<glm::vec4> &newColors) override;
	void getIndices(std::vector<uint16_t> &newIndices) override;
	void getNormals(std::vector<glm::vec3> &newNormals) override;
	void getUVs(std::vector<glm::vec2> &newUVs) override;
	void getVertices(std::vector<glm::vec3> &newVertices) override;

public:
	[[nodiscard]] OpenGL* getRender() const { return render; }

	void setRender(OpenGL* newRender) { this->render = newRender; }
};
#endif


#endif //SCENEMESH_H
