#version 430 core
layout (location = 0) in vec3 aPos;

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct MeshInstanceData {
	mat4 modelViewMatrix;
	Material material;
	bool lightsEnabled;
};

layout(std430, binding = 6) buffer Instances {
	MeshInstanceData data[];
};

void main(){
	MeshInstanceData meshData = data[gl_InstanceID];
	gl_Position = meshData.modelViewMatrix * vec4(aPos, 1.0);
}