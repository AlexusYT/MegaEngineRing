//
// Created by alexus on 13.02.24.
//

#include "GL.h"

#include <epoxy/gl.h>

namespace n::sdk::renderer {

void GL::viewport(const int pX, const int pY, const int pWidth, const int pHeight) {
	glViewport(pX, pY, pWidth, pHeight);
}

void GL::setClearColor(const float pRed, const float pGreen, const float pBlue, const float pAlpha) {
	glClearColor(pRed, pGreen, pBlue, pAlpha);
}

void GL::clear(const ClearBits pMask) {
	uint32_t mask{};
	if ((pMask & ClearBits::COLOR_BUFFER_BIT) == ClearBits::COLOR_BUFFER_BIT) mask |= GL_COLOR_BUFFER_BIT;
	if ((pMask & ClearBits::DEPTH_BUFFER_BIT) == ClearBits::DEPTH_BUFFER_BIT) mask |= GL_DEPTH_BUFFER_BIT;
	if ((pMask & ClearBits::STENCIL_BUFFER_BIT) == ClearBits::STENCIL_BUFFER_BIT) mask |= GL_STENCIL_BUFFER_BIT;
	glClear(mask);
}

} // namespace n::sdk::renderer