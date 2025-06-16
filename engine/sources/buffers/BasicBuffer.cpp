//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 07.04.25.
//

#include "KwasarEngine/buffers/BasicBuffer.h"

#include <epoxy/gl.h>

namespace mer::sdk {
BasicBuffer::~BasicBuffer() { unintialize(); }

void BasicBuffer::reallocate(int64_t pNewSize, const void* pNewData) {
	allocatedSize = pNewSize;
	if (name != 0) { glDeleteBuffers(1, &name); }
	glCreateBuffers(1, &name);
	glNamedBufferData(name, pNewSize, pNewData, usage);
}

void BasicBuffer::bufferSubData(int32_t pOffset, int64_t pSize, const void* pData) const {
	glNamedBufferSubData(name, pOffset, pSize, pData);
}

void BasicBuffer::unintialize() {
	if (name != 0) {
		glDeleteBuffers(1, &name);
		name = 0;
	}
}
} // namespace mer::sdk