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
