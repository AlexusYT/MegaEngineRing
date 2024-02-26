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