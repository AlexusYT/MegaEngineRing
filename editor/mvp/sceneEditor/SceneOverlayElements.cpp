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
// Created by alexus on 06.01.25.
//

#include "SceneOverlayElements.h"

#include <epoxy/gl.h>

#include "graphic/shaders/SimpleShader.h"

namespace ked {
SceneOverlayElements::SceneOverlayElements() {}

std::shared_ptr<SceneOverlayElements> SceneOverlayElements::create() {
	return std::shared_ptr<SceneOverlayElements>(new SceneOverlayElements());
}

void SceneOverlayElements::render() { if (gridVisible) renderGrid(); }

void SceneOverlayElements::renderGrid() const {
	shader->use();
	glBindVertexArray(vao);
	shader->setUniform("modelMatrix", glm::mat4(1.0f));
	glDrawElements(GL_LINES, static_cast<int32_t>(indices.size()), GL_UNSIGNED_SHORT, nullptr);

	shader->setUniform("modelMatrix", glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
	glDrawElements(GL_LINES, static_cast<int32_t>(indices.size()), GL_UNSIGNED_SHORT, nullptr);
	//glBindVertexArray(0);
}

ke::ReportMessagePtr SceneOverlayElements::onInitialize() {
	shader = SimpleShader::getInstance();
	if (auto msg = shader->initialize()) return msg;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &dataBuffer);
	glGenBuffers(1, &indexBuffer);
	glBindVertexArray(vao);
	const int MAX_X = 300;
	const int MAX_Z = 300;
	const int MIN_X = -300;
	const int MIN_Z = -300;
	std::vector<float> data;
	for (int x = MIN_X; x < MAX_X + 1; x++) {
		data.emplace_back(x);
		data.emplace_back(0);
		data.emplace_back(MAX_Z);
		indices.emplace_back(data.size() / 3 - 1);
		data.emplace_back(x);
		data.emplace_back(0);
		data.emplace_back(MIN_Z);
		indices.emplace_back(data.size() / 3 - 1);

		//indices.emplace_back(std::min(x + 1, 99));
	}
	for (int z = MIN_Z; z < MAX_Z + 1; z++) {
		data.emplace_back(MAX_X);
		data.emplace_back(0);
		data.emplace_back(z);
		indices.emplace_back(data.size() / 3 - 1);
		data.emplace_back(MIN_X);
		data.emplace_back(0);
		data.emplace_back(z);
		indices.emplace_back(data.size() / 3 - 1);
		//indices.emplace_back(x * X + z);
		//indices.emplace_back((x + 1) * X + z);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(indices.size() * sizeof(uint16_t)), indices.data(),
				 GL_STATIC_DRAW);
	int stride = (3 /*+ 3 + 2*/) * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);
	//Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	glEnableVertexAttribArray(0);


	/*
	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(5 * sizeof(float)));
	glEnableVertexAttribArray(2);*/
	glBindVertexArray(0);
	return nullptr;
}

void SceneOverlayElements::onUninitialize() { shader->uninitialize(); }
} // namespace ked
