#version 460 core

layout(location = 1) in vec3 vertexPosition_in;
layout(location = 2) in vec3 color_in;

uniform mat4 MVPMatrix;
out vec3 color_out;

void main(){
	gl_Position = MVPMatrix * vec4(vertexPosition_in, 1);
	color_out = color_in;
}