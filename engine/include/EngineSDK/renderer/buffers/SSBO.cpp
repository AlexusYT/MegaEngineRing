//
// Created by alexus on 04.02.24.
//

#include "SSBO.h"

#ifdef EDITOR_SDK


	#include <epoxy/gl.h>
#else


	#include <GL/glew.h>
#endif

namespace n::sdk::renderer {
SSBO::SSBO() {}

SSBO::~SSBO() {}

void SSBO::setData(const void* pData, size_t pSize, BufferUsageEnum pUsage) const {}

void SSBO::bind() const {}

void SSBO::unbind() const {}

uint32_t SSBO::native() const {}

void SSBO::bindBufferBase(uint32_t pBinding) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pSsbo->getBindingPoint(), pSsbo->native());
}
} // namespace n::sdk::renderer