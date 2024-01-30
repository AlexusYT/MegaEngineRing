#version 430 core

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec4 colorsIn;
layout (location = 3) in vec3 normalsIn;


layout(std430, binding = 2) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
};

struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct MeshInstanceData {
	mat4 modelViewMatrix;
	Material material;
	int type;
	bool lighted;
};

layout(std430, binding = 6) buffer Instances {
	MeshInstanceData data[];
};

out VS_OUT {
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec3 fragmentPos;
} vs_out;

out flat int instanceId;

bool enableSegment(int seg){
	if (seg == 7 && gl_VertexID >= 42 &&gl_VertexID <= 52) return true;
	if (gl_VertexID >= 6*seg &&gl_VertexID <= 6*(seg+1)-1) return true;
	return false;
}

void main(){
	instanceId=gl_InstanceID;
	MeshInstanceData meshData = data[instanceId];

	gl_Position = viewProjMatrix * meshData.modelViewMatrix * vec4(vertexIn, 1);
	vs_out.uv = uvIn;

	if ((meshData.type & 1) == 1){
		bool enabled = false;
		if (((meshData.type>>1) & 1) == 1){
			enabled = enableSegment(0);
		}
		if (((meshData.type>>2) & 1) == 1){
			enabled = enabled || enableSegment(1);
		}
		if (((meshData.type>>3) & 1) == 1){
			enabled = enabled || enableSegment(2);
		}
		if (((meshData.type>>4) & 1) == 1){
			enabled = enabled || enableSegment(3);
		}
		if (((meshData.type>>5) & 1) == 1){
			enabled = enabled || enableSegment(4);
		}
		if (((meshData.type>>6) & 1) == 1){
			enabled = enabled || enableSegment(5);
		}
		if (((meshData.type>>7) & 1) == 1){
			enabled = enabled || enableSegment(6);
		}
		if (((meshData.type>>8) & 1) == 1){
			enabled = enabled || enableSegment(7);
		}
		if (enabled){
			vs_out.color = vec4(0, 0.6, 0, 1);
		} else {
			vs_out.color = vec4(0, 0, 0, 1);
		}

	} else {
		vs_out.color = colorsIn;
	}
	/*if (meshData.type >= 1 && meshData.type <= 11){
		int digit = meshData.type-1;
		bool enabled;


		if (enabled){
			vs_out.color = vec4(0, 1, 0, 1);
		} else {
			vs_out.color = vec4(0, 0, 0, 1);
		}
	} else*/

	vs_out.normal = transpose(inverse(mat3(meshData.modelViewMatrix))) * normalsIn;

	vs_out.fragmentPos = vec3(meshData.modelViewMatrix * vec4(vertexIn, 1));

}