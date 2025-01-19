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
// Created by alexus on 26.07.23.
//

#ifndef MAIN_MESHBOUNDSBASE_H
#define MAIN_MESHBOUNDSBASE_H

#include <Render/opengl/buffers/VertexArrayObjectFwd.h>

#include "renderers/renderBuffers/RenderColorBuffer.h"
#include "renderers/renderBuffers/RenderIndicesBuffer.h"
#include "renderers/renderBuffers/RenderNormalsBuffer.h"
#include "renderers/renderBuffers/RenderUVsBuffer.h"
#include "renderers/renderBuffers/RenderVertexBuffer.h"

class MeshBoundsBase : public RenderIndicesBuffer, public RenderVertexBuffer, public RenderColorBuffer {
	rel::opengl::VertexArrayObjectSPtr renderBuffer;
	uint16_t expectedVerticesCount = 0;

protected:
	[[nodiscard]] uint16_t getExpectedVerticesCount() const { return expectedVerticesCount; }

	void drawElements();

	void updateBuffers() {
		if (!renderBuffer) initBuffers();
		RenderIndicesBuffer::updateBuffer();
		RenderVertexBuffer::updateBuffer();
		RenderColorBuffer::updateBuffer();
	}

	void invalidateAll() {
		onInvalidateAllStarted();
		invalidateIndices();
		invalidateVertices();
		invalidateColors();
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
		DEBUG_ASSERT(expectedVerticesCount <= RenderVertexBuffer::getBufferData().size(), PrintHandler{},
					 "Index value is greater than count of the vertices");
	}

	void invalidateColors() {
		RenderColorBuffer::invalidate();
		DEBUG_ASSERT(expectedVerticesCount <= RenderColorBuffer::getBufferData().size(), PrintHandler{},
					 "Index value is greater than count of the vertex colors");
	}

	void initBuffers();
};


#endif //MAIN_MESHBOUNDSBASE_H
