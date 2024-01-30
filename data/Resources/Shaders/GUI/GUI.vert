#version 430
const int VIEW_RENDER_TEXT = 1;
const int VIEW_RENDER_GRADIENT = 2;
const int VIEW_RENDER_COLORED_TEXTURE = 3;

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec4 colorsIn;
layout (location = 3) in int type;

layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
};

layout(std430, binding = 1) buffer ModelWideSettings {
	mat4 modelViewMatrix;
	vec4 viewPosStart;
	vec4 viewPosEnd;
};

out vec2 uvVS;
out vec4 colorVS;
flat out int typeVS;

void main() {
	gl_Position = viewProjMatrix * modelViewMatrix * vec4(vertexIn, 1.0);
	typeVS = type;
	switch (type){
		case VIEW_RENDER_COLORED_TEXTURE:
		case VIEW_RENDER_TEXT:{
			uvVS = uvIn;
			colorVS = colorsIn;
			break;
		}
		case VIEW_RENDER_GRADIENT:{
			colorVS = colorsIn;
			break;
		}
		default :{
			colorVS = vec4(1, 0, 1, 1);
			break;
		}

	}
}