//
// Created by alexus on 12.02.24.
//

#include "BasicRenderObject.h"

#include <epoxy/gl.h>
#include <glm/detail/type_vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "EngineSDK/main/resources/ResourceRequests.h"
#include "EngineSDK/main/resources/Resources.h"
#include "EngineSDK/main/resources/shaders/BuiltInProgramRequest.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"

namespace n::sdk::main {
using namespace engine::utils;

GLuint vao;

void BasicRenderObject::fillResourceRequests(const std::shared_ptr<ResourceRequests> &pRequests) {
	pRequests->addRequest(BuiltInProgramRequest::getDefaultProgram());
}

ReportMessagePtr BasicRenderObject::init() {
	setShader(getResources()->getByRequest(BuiltInProgramRequest::getDefaultProgram()));

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left
		0.5f,  -0.5f, 0.0f, // Right
		0.0f,  0.5f,  0.0f	// Top
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return nullptr;
}

void BasicRenderObject::render() {


	getShader()->use();
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
} // namespace n::sdk::main