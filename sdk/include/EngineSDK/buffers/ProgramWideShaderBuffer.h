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
// Created by alexus on 06.02.24.
//

#ifndef PROGRAMWIDESHADERBUFFER_H
#define PROGRAMWIDESHADERBUFFER_H

#include <glm/mat4x4.hpp>

#include "SSBO.h"

namespace mer::sdk {
enum class RenderPassMode : int32_t { REGULAR, OUTLINE };

class ProgramWideShaderBuffer : public SSBO {
	struct Data {
		glm::mat4 viewProjMatrix{1};
		glm::mat4 projectionMatrix{1};
		glm::mat4 viewMatrix{1};
		glm::vec4 camPos{0};
		int32_t mode{0};
	};

	Data data{};
	bool dirty{};

public:
	ProgramWideShaderBuffer() {
		bind();
		setData(&data, sizeof(data), DYNAMIC_DRAW);
	}

	void update() {
		if (!dirty) return;

		bind();
		bufferSubData(0, sizeof(data), &data);
		dirty = false;
	}

	[[nodiscard]] const glm::mat4 &getViewProjMatrix() const { return data.viewProjMatrix; }

	void setViewProjMatrix(const glm::mat4 &pViewProjMatrix) {
		data.viewProjMatrix = pViewProjMatrix;
		dirty = true;
	}

	void setMode(RenderPassMode pMode) {
		data.mode = static_cast<int32_t>(pMode);
		dirty = true;
	}

	void setCameraPos(const glm::vec3 &pPosition) {
		data.camPos = glm::vec4(pPosition, 1.0f);
		dirty = true;
	}
};
} // namespace mer::sdk


#endif //PROGRAMWIDESHADERBUFFER_H
