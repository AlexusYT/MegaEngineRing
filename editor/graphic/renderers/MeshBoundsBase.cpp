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
// Created by alexus on 26.07.23.
//

#include "MeshBoundsBase.h"

#include <Render/opengl/buffers/VertexArrayObject.h>

using namespace rel::opengl;

void MeshBoundsBase::drawElements() {
	renderBuffer->drawElementsBind(RenderIndicesBuffer::getBufferData().size(), LINES);
}

void MeshBoundsBase::initBuffers() {

	auto renderBufferBuilder = rel::opengl::VertexArrayObject::Builder()
								   .addBuffer(RenderIndicesBuffer::getBufferBuilder())
								   .addBuffer(RenderVertexBuffer::getBufferBuilder(0))
								   .addBuffer(RenderColorBuffer::getBufferBuilder(2));
	renderBuffer = renderBufferBuilder.build();
	RenderIndicesBuffer::setBuffer(renderBuffer->getIBOs()[0]);
	RenderVertexBuffer::setBuffer(renderBuffer->getVBOs()[0]);
	RenderColorBuffer::setBuffer(renderBuffer->getVBOs()[1]);
	invalidateAll();
}
