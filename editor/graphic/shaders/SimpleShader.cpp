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

#include "SimpleShader.h"

#include "EngineSDK/resources/shaders/Shader.h"

namespace mer::editor::graphics {
std::shared_ptr<SimpleShader> SimpleShader::instance = std::shared_ptr<SimpleShader>(new SimpleShader);

SimpleShader::SimpleShader() {
	Resource::setResourceUri("_builtin_/shaders/SimpleShader.enshader");

	std::shared_ptr<sdk::Shader> vertexShader = std::make_shared<sdk::VertexShader>();
	//language=glsl
	vertexShader->setSource(R"(
#version 460 core
layout (location = 0) in vec3 vertexIn;
uniform mat4 modelMatrix;
layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec3 camPos;
};


void main(){

	gl_Position = vec4(vertexIn, 1);
})");
	attachShader(vertexShader);

	std::shared_ptr<sdk::Shader> geometryShader = std::make_shared<sdk::GeometryShader>();
	//language=glsl
	geometryShader->setSource(R"(
#version 460 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;
layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec3 camPos;
};

uniform mat4 modelMatrix;
void main() {
	//vec3 a = vec3(0, 0.0f, 0.07f);
	vec3 a = vec3(0.002f, 0.0f, 0.002f);
    mat3 normalMatrix = mat3(transpose(inverse(viewMat * modelMatrix)));
    //vs_out.normal = 			  vec3(projectionMat*vec4(normalMatrix * aNormal, 0.0));
    gl_Position = viewProjectionMat * modelMatrix *vec4(gl_in[0].gl_Position.xyz+a, 1.0f);
    EmitVertex();

    gl_Position = viewProjectionMat * modelMatrix * vec4(gl_in[0].gl_Position.xyz-a, 1.0f);
    EmitVertex();

    gl_Position = viewProjectionMat * modelMatrix * vec4(gl_in[1].gl_Position.xyz+a, 1.0f);
    EmitVertex();

    gl_Position = viewProjectionMat * modelMatrix * vec4(gl_in[1].gl_Position.xyz-a, 1.0f);
    EmitVertex();

    EndPrimitive();
}
)");
	attachShader(geometryShader);
	std::shared_ptr<sdk::Shader> fragmentShader = std::make_shared<sdk::FragmentShader>();
	//language=glsl
	fragmentShader->setSource(R"(
#version 460 core
out vec4 outputColor;


layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec3 camPos;
};

void main()
{
	outputColor = vec4(0.2, 0.2, 0.2, 0.5);
})");
	attachShader(fragmentShader);
}

std::shared_ptr<SimpleShader> SimpleShader::getInstance() { return instance; }
} // namespace mer::editor::graphics