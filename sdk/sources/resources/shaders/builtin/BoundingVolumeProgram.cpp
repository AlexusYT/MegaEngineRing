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
// Created by alexus on 06.03.25.
//

#include "EngineSDK/resources/shaders/builtin/BoundingVolumeProgram.h"

#include "EngineSDK/resources/shaders/Shader.h"

namespace mer::sdk {

std::shared_ptr<BoundingVolumeProgram> BoundingVolumeProgram::instance =
	std::shared_ptr<BoundingVolumeProgram>(new BoundingVolumeProgram);

BoundingVolumeProgram::BoundingVolumeProgram() {
	Resource::setResourceUri("_builtin_/shaders/BoundingVolumeProgram.enshader");

	std::shared_ptr<Shader> vertexShader = std::make_shared<VertexShader>();
	//language=glsl
	vertexShader->setSource(R"(
#version 460 core
layout (location = 0) in vec3 vertexIn;
layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec4 camPos;
	int mode;
};

void main(){
	gl_Position = viewProjectionMat * vec4(vertexIn, 1);
}
)");
	attachShader(vertexShader);

	std::shared_ptr<Shader> fragmentShader = std::make_shared<FragmentShader>();
	//language=glsl
	fragmentShader->setSource(R"(
#version 460 core
out vec4 outputColor;
uniform vec4 color;

layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec4 camPos;
	int mode;
};

void main() {
		outputColor = color;
})");
	attachShader(fragmentShader);
}

std::shared_ptr<BoundingVolumeProgram> BoundingVolumeProgram::getInstance() { return instance; }
} // namespace mer::sdk