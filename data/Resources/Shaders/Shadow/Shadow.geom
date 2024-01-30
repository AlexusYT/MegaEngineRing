#version 430 core
layout(invocations = 16) in;
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

struct Light{
	mat4 shadowMatrices[6];
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

layout(std430, binding = 7) buffer LightData {
	Light lights[];
};

out vec3 FragPos;
out vec3 LightPos;

void main(){
	int id = gl_InvocationID;
	Light light = lights[id];
	for (int face = 0; face < 6; ++face){
		mat4 shadowMatrix = light.shadowMatrices[face];
		gl_Layer = id * 6 + face;// built-in variable that specifies to which face we render.*/
		for (int i = 0; i < 3; ++i){ // for each triangle's vertices
			LightPos = light.position.xyz;
			vec4 FragPosTmp = gl_in[i].gl_Position;
			FragPos = FragPosTmp.xyz;
			gl_Position = shadowMatrix * FragPosTmp;
			EmitVertex();
		}
		EndPrimitive();
	}
}