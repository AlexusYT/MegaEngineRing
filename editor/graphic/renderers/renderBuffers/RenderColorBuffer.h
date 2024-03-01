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

#ifndef MAIN_RENDERCOLORBUFFER_H
#define MAIN_RENDERCOLORBUFFER_H

#include <Render/opengl/buffers/VertexBufferObject.h>

#include "RenderBuffer.h"

class RenderColorBuffer : protected RenderBuffer<glm::vec4, rel::opengl::VertexBufferObjectSPtr> {
protected:
	virtual void getColors(std::vector<glm::vec4> &newColors) = 0;

	virtual void onColorsInvalidated() {}

private:
	void getData(std::vector<ElementT> &data) override { getColors(data); }

	void onInvalidated() override { onColorsInvalidated(); }
};


#endif //MAIN_RENDERCOLORBUFFER_H
