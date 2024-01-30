#version 430 core
vec3 applyGamma(in vec3 color);

layout(std430, binding = 2) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
};



out vec4 colorOut;
in VS_OUT {
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec3 fragmentPos;
} gs_in;



void main(){
	vec3 result = vec3(0, 0, 0);
	colorOut = gs_in.color;

}
