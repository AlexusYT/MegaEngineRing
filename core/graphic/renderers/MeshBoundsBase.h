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
