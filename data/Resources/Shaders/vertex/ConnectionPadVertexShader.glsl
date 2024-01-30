#version 460 core

layout(location = 1) in vec3 vertexPosition;

uniform mat4 MVPMatrix;

void main(){
	gl_Position = MVPMatrix * vec4(vertexPosition, 1);
}