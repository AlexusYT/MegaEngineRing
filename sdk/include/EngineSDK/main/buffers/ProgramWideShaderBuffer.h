//
// Created by alexus on 06.02.24.
//

#ifndef PROGRAMWIDESHADERBUFFER_H
#define PROGRAMWIDESHADERBUFFER_H
#include <EngineSDK/renderer/buffers/SSBO.h>
#include <glm/mat4x4.hpp>

namespace mer::sdk::main {
class ProgramWideShaderBuffer : public renderer::SSBO {

	glm::mat4 viewProjMatrix{1};

public:
	ProgramWideShaderBuffer() {
		SSBO::bind();
		SSBO::setData(&viewProjMatrix, sizeof(viewProjMatrix), renderer::BufferUsageEnum::DYNAMIC_DRAW);
	}

	[[nodiscard]] const glm::mat4 &getViewProjMatrix() const { return viewProjMatrix; }

	void setViewProjMatrix(const glm::mat4 &pViewProjMatrix) {
		viewProjMatrix = pViewProjMatrix;
		bind();
		bufferSubData(0, sizeof(viewProjMatrix), &viewProjMatrix);
	}
};
} // namespace mer::sdk::main


#endif //PROGRAMWIDESHADERBUFFER_H
