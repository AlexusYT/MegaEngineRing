// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 04.02.24.
//

#include "SSBO.h"

#include <epoxy/gl.h>

namespace mer::sdk::renderer {
SSBO::SSBO() { glGenBuffers(1, &name); }

SSBO::~SSBO() { glDeleteBuffers(1, &name); }

void SSBO::setData(const void* pData, const int64_t pSize, const BufferUsageEnum pUsage) const {
	bind();
	glBufferData(GL_SHADER_STORAGE_BUFFER, pSize, pData, pUsage);
}

void SSBO::bind() const { glBindBuffer(GL_SHADER_STORAGE_BUFFER, name); }

void SSBO::unbind() const {}

uint32_t SSBO::native() const { return name; }

void SSBO::bindBufferBase(const uint32_t pBinding) { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pBinding, name); }

void SSBO::bufferSubData(int32_t pOffset, int64_t pSize, const void* pData) {
	bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, pOffset, pSize, pData);
}
} // namespace mer::sdk::renderer