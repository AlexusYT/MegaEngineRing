//
// Created by alexus on 25.07.23.
//

#ifndef MAIN_RENDERUVSBUFFER_H
#define MAIN_RENDERUVSBUFFER_H

#include "RenderBuffer.h"

class RenderUVsBuffer : protected RenderBuffer<glm::vec2, rel::opengl::VertexBufferObjectSPtr> {
protected:
	virtual void getUVs(std::vector<glm::vec2> &newUVs) = 0;

	virtual void onUVsInvalidated() {}

private:
	void getData(std::vector<ElementT> &data) override { getUVs(data); }

	void onInvalidated() override { onUVsInvalidated(); }
};


#endif //MAIN_RENDERUVSBUFFER_H
