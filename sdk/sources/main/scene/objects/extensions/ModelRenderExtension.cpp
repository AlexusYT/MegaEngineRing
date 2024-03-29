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
// Created by alexus on 29.03.24.
//

#include "EngineSDK/main/scene/objects/extensions/ModelRenderExtension.h"

#include <epoxy/gl.h>
#include <EngineSDK/main/buffers/ProgramWideShaderBuffer.h>
#include <EngineSDK/main/resources/MultipleResource.h>
#include <EngineSDK/main/resources/models/ModelResource.h>
#include <EngineSDK/main/resources/models/ObjModelRequest.h>
#include <EngineSDK/main/resources/shaders/ShaderProgramRequest.h>
#include <EngineSDK/renderer/shaders/ShaderProgram.h>

#include "EngineUtils/utils/Logger.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"

namespace mer::sdk::main {
utils::ReportMessagePtr ModelRenderExtension::onInit() {
	enqueueResourceLoading(shaderRequest, [this](const std::shared_ptr<renderer::ShaderProgram> &pProgram,
												 const utils::ReportMessagePtr &pError) {
		if (pError) {
			utils::Logger::error(pError);
			return;
		}
		shader = pProgram;
	});
	enqueueResourceLoading(modelRequest.first, [this](const std::shared_ptr<MultipleResource> &pModel,
													  const utils::ReportMessagePtr &pError) {
		if (pError) {
			utils::Logger::error(pError);
			return;
		}

		if (auto iter = pModel->find(modelRequest.second); iter == pModel->end()) {

			auto msg = utils::ReportMessage::create();
			msg->setTitle("Failed to get model object with specified name");
			msg->setMessage("No model object resource with such name");
			msg->addInfoLine("Resource name: {}", modelRequest.second);
			utils::Logger::error(msg);
		} else {
			if (auto modelObject = std::dynamic_pointer_cast<ModelResource>(iter->second)) {
				model = modelObject;
			} else {
				auto msg = utils::ReportMessage::create();
				msg->setTitle("Failed to get model object with specified name");
				msg->setMessage("Resource type is incorrect");
				msg->addInfoLine("Resource name: {}", modelRequest.second);
				msg->addInfoLine("Actual type is: {}", Utils::getTypeName(iter->second.get()));
				msg->addInfoLine("Expected type is: {}", Utils::getTypeName(iter->second.get()));
				utils::Logger::error(msg);
			}
		}
	});
	return nullptr;
}

utils::ReportMessagePtr ModelRenderExtension::onDeinit() {
	shader.reset();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	return nullptr;
}

void ModelRenderExtension::onRender() {
	if (!shader || !model) return;
	static bool b{};
	if (!b) {
		shader->attachSsbo(getObject()->getScene()->getProgramBuffer(), "ProgramWideSettings", 0);
		setupBuffers();
		b = true;
	}
	shader->use();
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, static_cast<int32_t>(model->getIndices().size()), GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
}

void ModelRenderExtension::setupBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	auto &indices = model->getIndices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(indices.size() * sizeof(uint16_t)), indices.data(),
				 GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	auto &vertices = model->getVertices();
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(vertices.size() * sizeof(glm::vec3)), vertices.data(),
				 GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
} // namespace mer::sdk::main