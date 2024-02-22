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
