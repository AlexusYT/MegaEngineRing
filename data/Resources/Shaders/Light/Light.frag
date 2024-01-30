#version 430 core
vec3 applyGamma(in vec3 color);


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

uniform samplerCubeArray depthMap;


out vec4 colorOut;
in vec2 uv;
in vec3 normal;
in vec4 color;
in vec3 fragmentPos;
uniform sampler2D sampler;


float ShadowCalculation(Light light, in int lightIndex, vec3 fragPos)
{
	float far_plane = 25;
	// get vector between fragment position and light position
	vec3 fragToLight = fragPos - light.position.xyz;
	// ise the fragment to light vector to sample from the depth map
	float closestDepth = texture(depthMap, vec4(fragToLight, lightIndex)).r;
	// it is currently in linear range between [0,1], let's re-transform it back to original depth value
	closestDepth *= far_plane;
	// now get current linear depth as the length between the fragment and light position
	float currentDepth = length(fragToLight);
	// test for shadows
	float bias = max(0.05 * (1.0 - dot(normal, light.direction.xyz)), 0.005);
	//float bias = 0.05;// we use a much larger bias since depth is now in [near_plane, far_plane] range
	float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	// display closestDepth as debug (to visualize depth cubemap)
	//color = vec3(closestDepth / far_plane);

	return shadow;
}

vec3 addLight(Light light, in int lightIndex, vec3 fragmentPos);
void main(){
	if (!lightsEnabled) colorOut = color;
	else {
		vec3 result = vec3(0, 0, 0);
		for (int i = 0; i < lightsCount; i++) result += addLight(lights[i], i, fragmentPos);

		colorOut = vec4(applyGamma(result), 1) * color;
	}
}

vec3 addLight(Light light, in int lightIndex, vec3 fragmentPos){

	vec3 lightDir = normalize(light.position.xyz - fragmentPos);

	vec3 ambient = light.ambient.xyz * material.ambient.xyz;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse =  light.diffuse.xyz * diff * material.diffuse.xyz;

	vec3 viewDir = normalize(-fragmentPos);// the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular.xyz * (spec * material.specular.xyz);

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

	float shadow = ShadowCalculation(light, lightIndex, fragmentPos);
	vec3 result = (1.0 - shadow) * (ambient + diffuse + specular);;
	return result;
}
