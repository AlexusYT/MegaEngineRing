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
// Created by alexus on 25.07.23.
//

#ifndef MAIN_RENDERVERTEXBUFFER_H
#define MAIN_RENDERVERTEXBUFFER_H

#include "RenderBuffer.h"

class RenderVertexBuffer : protected RenderBuffer<glm::vec3, rel::opengl::VertexBufferObjectSPtr> {
protected:
	virtual void getVertices(std::vector<glm::vec3> &newVertices) = 0;

	virtual void onVerticesInvalidated() {}

private:
	void getData(std::vector<ElementT> &data) override { getVertices(data); }

	void onInvalidated() override { onVerticesInvalidated(); }
};


#endif //MAIN_RENDERVERTEXBUFFER_H
