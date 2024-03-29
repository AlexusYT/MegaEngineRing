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

#ifndef MAIN_SCENEMESHBASE_H
#define MAIN_SCENEMESHBASE_H
#if 0

	#include <Render/opengl/buffers/VertexArrayObjectFwd.h>

	#include "graphic/renderers/renderBuffers/RenderColorBuffer.h"
	#include "graphic/renderers/renderBuffers/RenderIndicesBuffer.h"
	#include "graphic/renderers/renderBuffers/RenderNormalsBuffer.h"
	#include "graphic/renderers/renderBuffers/RenderUVsBuffer.h"
	#include "graphic/renderers/renderBuffers/RenderVertexBuffer.h"

class SceneMeshBase : public RenderIndicesBuffer,
					  public RenderVertexBuffer,
					  public RenderColorBuffer,
					  public RenderUVsBuffer,
					  public RenderNormalsBuffer {

	rel::opengl::VertexArrayObjectSPtr renderBuffer;
	uint16_t expectedVerticesCount = 0;

protected:
	[[nodiscard]] uint16_t getExpectedVerticesCount() const { return expectedVerticesCount; }

	void drawElements();

	void updateBuffers() {
		RenderIndicesBuffer::updateBuffer();
		RenderVertexBuffer::updateBuffer();
		RenderUVsBuffer::updateBuffer();
		RenderColorBuffer::updateBuffer();
		RenderNormalsBuffer::updateBuffer();
	}

	void invalidateAll() {
		onInvalidateAllStarted();
		invalidateIndices();
		invalidateVertices();
		invalidateUVs();
		invalidateColors();
		invalidateNormals();
	}

	virtual void onInvalidateAllStarted() {}

	void invalidateIndices() {
		RenderIndicesBuffer::invalidate();
		const auto &indicesData = RenderIndicesBuffer::getBufferData();
		if (!indicesData.empty()) expectedVerticesCount = *std::max_element(indicesData.begin(), indicesData.end()) + 1;
		else
			expectedVerticesCount = 0;
	}

	void invalidateVertices() {
		RenderVertexBuffer::invalidate();
		/*DEBUG_ASSERT(expectedVerticesCount <= RenderVertexBuffer::getBufferData().size(), PrintHandler{},
					 "Index value is greater than count of the vertices");*/
	}

	void invalidateUVs() {
		RenderUVsBuffer::invalidate();
		/*DEBUG_ASSERT(expectedVerticesCount <= RenderUVsBuffer::getBufferData().size(), PrintHandler{},
					 "Index value is greater than count of the vertex UVs");*/
	}

	void invalidateColors() {
		RenderColorBuffer::invalidate();
		/*DEBUG_ASSERT(expectedVerticesCount <= RenderColorBuffer::getBufferData().size(), PrintHandler{},
					 "Index value is greater than count of the vertex colors");*/
	}

	void invalidateNormals() {
		RenderNormalsBuffer::invalidate();
		/*DEBUG_ASSERT(expectedVerticesCount <= RenderNormalsBuffer::getBufferData().size(), PrintHandler{},
					 "Index value is greater than count of the vertex normals");*/
	}

	void initBuffers();

	rel::opengl::VertexArrayObject* getRenderBuffer() const { return renderBuffer.get(); }
};

#endif


#endif //MAIN_SCENEMESHBASE_H
