//
// Created by alexus on 25.07.23.
//

#ifndef MAIN_RENDERCOLORBUFFER_H
#define MAIN_RENDERCOLORBUFFER_H

#include <Render/opengl/buffers/VertexBufferObject.h>

#include "RenderBuffer.h"

class RenderColorBuffer : protected RenderBuffer<glm::vec4, rel::opengl::VertexBufferObjectSPtr> {
protected:
	virtual void getColors(std::vector<glm::vec4> &newColors) = 0;

	virtual void onColorsInvalidated() {}

private:
	void getData(std::vector<ElementT> &data) override { getColors(data); }

	void onInvalidated() override { onColorsInvalidated(); }
};


#endif //MAIN_RENDERCOLORBUFFER_H
