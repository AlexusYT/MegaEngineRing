//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 13.02.24.
//

#include "EngineSDK/renderer/GL.h"

#include <epoxy/gl.h>

namespace mer::sdk::renderer {

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

} // namespace mer::sdk::renderer