#version 430 core

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec4 colorsIn;
layout (location = 3) in vec3 normalsIn;


layout(std430, binding = 2) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
};

/*layout(std430, binding = 3) buffer ModelWideSettings {
	mat4 modelViewMatrix;
};*/

out VS_OUT {
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec3 fragmentPos;
} vs_out;



void main(){

	mat4 modelViewMatrix = mat4(1);
	gl_Position = viewProjMatrix * modelViewMatrix * vec4(vertexIn, 1);
	vs_out.uv = uvIn;
	vs_out.normal = transpose(inverse(mat3(modelViewMatrix))) * normalsIn;

	vs_out.fragmentPos = vec3(modelViewMatrix * vec4(vertexIn, 1));

}