#version 430 core

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec4 colorsIn;
layout (location = 3) in vec3 normalsIn;


struct Light{
	vec4 position;
	vec4 direction;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float cutOff;
	float outerCutOff;
	float linear;
	float quadratic;
	int type;
};

layout(std430, binding = 2) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
	Light lights[40];
	int lightsCount;
};

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

layout(std430, binding = 3) buffer ModelWideSettings {
	mat4 modelViewMatrix;
	Material material;
	bool lightsEnabled;
};

out vec2 uv;
out vec3 normal;
out vec3 fragmentPos;
out vec4 color;
//out vec3 LightPos;

uniform mat4 viewMatrix;
uniform mat3 normalTransformMatrix;
//uniform vec3 lightPos;

void main(){
	gl_Position = viewProjMatrix * modelViewMatrix * vec4(vertexIn, 1);
	uv = uvIn;
	color = colorsIn;
	normal = transpose(inverse(mat3(modelViewMatrix))) * normalsIn;

	fragmentPos = vec3(modelViewMatrix * vec4(vertexIn, 1));
}