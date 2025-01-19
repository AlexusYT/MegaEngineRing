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
// Created by alexus on 27.02.24.
//

#include <EngineSDK/main/extensions/BasicRenderExtension.h>

#include <EngineUtils/utils/Logger.h>
#include <epoxy/gl.h>

#include "EngineSDK/main/resources/shaders/ShaderProgram.h"
#include "EngineSDK/main/scene/Scene.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"

namespace mer::sdk::main {
utils::ReportMessagePtr BasicRenderExtension::onInit() {
	/*enqueueResourceLoading(
		BuiltInProgramRequest::getDefaultProgram(),
		[this](const std::shared_ptr<renderer::ShaderProgram> &pProgram, const utils::ReportMessagePtr &pError) {
			if (pError) {
				utils::Logger::error(pError);
				return;
			}
			shader = pProgram;
		});*/
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLfloat vertices[] = {
		-10.5f, -10.5f, 0.0f, // Left
		10.5f,	-10.5f, 0.0f, // Right
		0.0f,	10.5f,	1.0f  // Top
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	return nullptr;
}

utils::ReportMessagePtr BasicRenderExtension::onDeinit() {
	shader.reset();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	return nullptr;
}

void BasicRenderExtension::onRender() {
	if (!shader) return;
	static bool b{};
	if (!b) {
		b = true;
	}
	shader->use();
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
} // namespace mer::sdk::main