//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 06.03.25.
//

#include "KwasarEngine/bounding/BoundingVolume.h"

#include <epoxy/gl.h>

namespace ke {
void BoundingVolume::render() {
	if (!isInited()) return;
	if (shouldUpdateIndices) {
		auto indices = getIndices();
		indicesCount = indices.size();
		if (indicesBuffer)
			glDeleteBuffers(1, &indicesBuffer);
		glCreateBuffers(1, &indicesBuffer);
		glNamedBufferData(indicesBuffer, static_cast<GLsizeiptr>(indices.size() * sizeof(uint16_t)), indices.data(),
						  GL_STATIC_DRAW);
		glVertexArrayElementBuffer(vao, indicesBuffer);
		shouldUpdateIndices = false;
	}
	if (shouldUpdateVertices) {
		if (verticesBuffer)
			glDeleteBuffers(1, &verticesBuffer);
		glCreateBuffers(1, &verticesBuffer);
		auto vertices = getVertices();
		glNamedBufferData(verticesBuffer, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(),
						  GL_STATIC_DRAW);
		glVertexArrayVertexBuffer(vao, 0, verticesBuffer, 0, sizeof(float) * 3);
		shouldUpdateVertices = false;
	}
	glBindVertexArray(vao);
	glDrawElements(GL_LINES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void BoundingVolume::updateVertices() { shouldUpdateVertices = true; }

void BoundingVolume::updateIndices() { shouldUpdateIndices = true; }

ReportMessagePtr BoundingVolume::onInitialize() {
	glCreateVertexArrays(1, &vao);

	uint32_t attribIndex = 0;
	uint32_t bindingIndex = attribIndex;
	glEnableVertexArrayAttrib(vao, attribIndex);
	glVertexArrayAttribBinding(vao, attribIndex, bindingIndex);
	glVertexArrayAttribFormat(vao, attribIndex, 3, GL_FLOAT, GL_FALSE, 0);

	return nullptr;
}

void BoundingVolume::onUninitialize() {
	if (indicesBuffer) {
		glDeleteBuffers(1, &indicesBuffer);
		indicesBuffer = 0;
	}
	if (verticesBuffer) {
		glDeleteBuffers(1, &verticesBuffer);
		verticesBuffer = 0;
	}
	if (vao) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}
} // namespace ke