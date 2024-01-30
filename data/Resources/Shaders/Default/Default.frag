#version 430 core
vec3 applyGamma(in vec3 color);


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
layout(std430, binding = 2) buffer ProgramWideSettings {
	mat4 viewProjMatrix;
};


uniform samplerCubeArray depthMap;
uniform sampler2D textureIn;


out vec4 colorOut;
in VS_OUT {
	vec2 uv;
	vec3 normal;
	vec4 color;
	vec3 fragmentPos;
} gs_in;

in flat int instanceIdOut;


float far_plane = 25;

float ShadowCalculation(Light light, in int lightIndex, vec3 fragPos)
{
	// get vector between fragment position and light position
	vec3 fragToLight = fragPos - light.position.xyz;
	// ise the fragment to light vector to sample from the depth map
	float closestDepth = texture(depthMap, vec4(fragToLight, lightIndex)).r;
	// it is currently in linear range between [0,1], let's re-transform it back to original depth value
	closestDepth *= far_plane;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// test for shadows
	float bias = max(0.05 * (1.0 - dot(gs_in.normal, light.direction.xyz)), 0.005);
	//float bias = 0.05;// we use a much larger bias since depth is now in [near_plane, far_plane] range
	float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	// display closestDepth as debug (to visualize depth cubemap)
	//colorOut = vec4(vec3(closestDepth / far_plane), 1.0);

	return shadow;
}

vec3 addLight(Light light, in MeshInstanceData meshData, in int lightIndex, vec3 fragmentPos);
void main(){
	MeshInstanceData meshData = data[instanceIdOut];
	if (!meshData.lighted) colorOut = gs_in.color;
	else {
		vec3 result = vec3(0, 0, 0);
		for (int i = 0; i < lights.length(); i++){
			result += addLight(lights[i], meshData, i, gs_in.fragmentPos);

		}
		colorOut = vec4(applyGamma(result), 1) * gs_in.color * vec4(texture(textureIn, gs_in.uv).xyz, 1);
	}
}

vec3 addLight(Light light, in MeshInstanceData meshData, in int lightIndex, vec3 fragmentPos){

	vec3 lightDir = normalize(light.position.xyz - fragmentPos);

	vec3 ambient = light.ambient.xyz * meshData.material.ambient.xyz;

	float diff = max(dot(gs_in.normal, lightDir), 0.0);
	vec3 diffuse =  light.diffuse.xyz * diff * meshData.material.diffuse.xyz;

	vec3 viewDir = normalize(-fragmentPos);// the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
	vec3 reflectDir = reflect(-lightDir, gs_in.normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), meshData.material.shininess);
	vec3 specular = light.specular.xyz * (spec * meshData.material.specular.xyz);

	if (light.type == 1){ // spotlight
		float theta = dot(lightDir, light.direction.xyz);
		float epsilon = (light.cutOff - light.outerCutOff);
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		diffuse  *= intensity;
		specular *= intensity;
	}

	float distance    = length(light.position.xyz - fragmentPos);
	float attenuation = 1.0 / (1 + light.linear * distance + light.quadratic * (distance * distance));

	float limit = 0.01;
	attenuation = (attenuation - limit) / (1 - limit);
	attenuation = max(attenuation, 0);

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	//float shadow = 0;
	float shadow = ShadowCalculation(light, lightIndex, fragmentPos);
	vec3 result = (1.0 - shadow) * (ambient + diffuse + specular);;
	return result;
}
