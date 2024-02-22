//
// Created by alexus on 25.07.23.
//
#if 0

	#include "SceneMeshBase.h"

	#include <Render/opengl/buffers/VertexArrayObject.h>

using namespace rel::opengl;

void SceneMeshBase::drawElements() { renderBuffer->drawElementsBind(RenderIndicesBuffer::getBufferData().size()); }

void SceneMeshBase::initBuffers() {

	auto renderBufferBuilder = rel::opengl::VertexArrayObject::Builder()
								   .addBuffer(RenderIndicesBuffer::getBufferBuilder())
								   .addBuffer(RenderVertexBuffer::getBufferBuilder(0))
								   .addBuffer(RenderUVsBuffer::getBufferBuilder(1))
								   .addBuffer(RenderColorBuffer::getBufferBuilder(2))
								   .addBuffer(RenderNormalsBuffer::getBufferBuilder(3));
	renderBuffer = renderBufferBuilder.build();
	RenderIndicesBuffer::setBuffer(renderBuffer->getIBOs()[0]);
	RenderVertexBuffer::setBuffer(renderBuffer->getVBOs()[0]);
	RenderUVsBuffer::setBuffer(renderBuffer->getVBOs()[1]);
	RenderColorBuffer::setBuffer(renderBuffer->getVBOs()[2]);
	RenderNormalsBuffer::setBuffer(renderBuffer->getVBOs()[3]);
}

#endif
