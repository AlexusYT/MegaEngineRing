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
