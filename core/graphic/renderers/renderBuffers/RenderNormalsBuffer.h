//
// Created by alexus on 25.07.23.
//

#ifndef MAIN_RENDERNORMALSBUFFER_H
#define MAIN_RENDERNORMALSBUFFER_H

#include "RenderBuffer.h"

class RenderNormalsBuffer : protected RenderBuffer<glm::vec3, rel::opengl::VertexBufferObjectSPtr> {
protected:
	virtual void getNormals(std::vector<glm::vec3> &newNormals) = 0;

	virtual void onNormalsInvalidated() {}

private:
	void getData(std::vector<ElementT> &data) override { getNormals(data); }

	void onInvalidated() override { onNormalsInvalidated(); }
};


#endif //MAIN_RENDERNORMALSBUFFER_H
