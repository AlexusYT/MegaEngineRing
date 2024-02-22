//
// Created by alexus on 25.07.23.
//

#ifndef MAIN_RENDERINDICESBUFFER_H
#define MAIN_RENDERINDICESBUFFER_H

#include "RenderBuffer.h"

class RenderIndicesBuffer : protected RenderBuffer<uint16_t, rel::opengl::IndexBufferObjectSPtr> {
protected:
	virtual void getIndices(std::vector<uint16_t> &newIndices) = 0;

	virtual void onIndicesInvalidated() {}

private:
	void getData(std::vector<ElementT> &data) override { getIndices(data); }

	void onInvalidated() override { onIndicesInvalidated(); }
};


#endif //MAIN_RENDERINDICESBUFFER_H
