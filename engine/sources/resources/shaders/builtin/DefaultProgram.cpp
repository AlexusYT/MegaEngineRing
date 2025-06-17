//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 16.02.25.
//

#include "KwasarEngine/resources/shaders/builtin/DefaultProgram.h"

#include "KwasarEngine/resources/shaders/Shader.h"

namespace ke {
std::shared_ptr<DefaultProgram> DefaultProgram::instance = std::shared_ptr<DefaultProgram>(new DefaultProgram);

DefaultProgram::DefaultProgram() {
	Resource::setResourceUri("_builtin_/shaders/DefaultProgram.enshader");

	std::shared_ptr<Shader> vertexShader = std::make_shared<VertexShader>();
	//language=glsl
	vertexShader->setSource(R"(
#version 460 core

struct Material {

	//xy - metallicRoughness texture handle, z - metallic factor, w - roughness factor
	vec4 metallicRoughnessInfo;
	//xy - texture handle, zw - not used
	vec4 baseColorInfo;
	//xyzw - base color factor
	vec4 baseColorFactor;

	//xy - texture handle, z - scale, w - not used
	vec4 normalInfo;
	//xy - texture handle, z - strength, w - not used
	vec4 occlusionInfo;
	//xy - texture handle, zw - not used
	vec4 emissiveInfo;
	//xyz - emissive color, w - not used
	vec4 emissiveFactor;
};

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in uint instanceIdIn;
layout (location = 2) in uint meshIdIn;
layout (location = 3) in vec3 normalsIn;
layout(std430, binding = 5) buffer UvSsbo {
	vec2 uvIn[];
};
/*layout(std430, binding = 6) buffer NormalSsbo {
	vec3 normalsIn[];
};*/
layout(std430, binding = 7) buffer ColorSsbo {
	vec4 colorsIn[];
};
uniform uint shaderSettings;
layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec4 camPos;
	int mode;
};

struct InstanceData {
	//Material material;
	mat4 modelMat;
	mat4 normalMat;
	int lightDataId;
};

layout(std430, binding = 1) buffer Instances {
	InstanceData instancesData[];
};

struct MeshInfo{
	uint uvStartPos;
	uint uvCount;
	uint colorStartPos;
	uint colorCount;
	uint materialId;
	uint instanceIdStart;
};
layout(std430, binding = 4) buffer MeshInfoSsbo {
	MeshInfo meshes[];
};


layout(std430, binding = 2) buffer Materials {
	Material materials[];
};
out flat int instanceId;
out vec2 UVCoords;
out vec3 WorldPos;
out vec3 Normal;
out vec4 Color;
out flat uint hasColors;
out flat Material material;

void main(){
	instanceId=gl_InstanceID;
	MeshInfo mesh = meshes[meshIdIn];
	InstanceData instanceData = instancesData[instanceIdIn];
	material = materials[mesh.materialId];
	UVCoords = uvIn[gl_VertexID-mesh.uvStartPos];
	if(mesh.colorCount==0){
		hasColors = 0;
	}else{
		hasColors = 1;
		Color = colorsIn[gl_VertexID-mesh.colorStartPos];
	}
	//vec3 normalTmp = normalsIn[gl_VertexID];
	vec3 normalTmp = normalsIn;
	Normal = mat3(instanceData.normalMat) * normalTmp;
	vec4 worldVertexPos = instanceData.modelMat* vec4(vertexIn, 1);
	if(mode == 1){
		vec3 scale = vec3(1.0f/length(instanceData.modelMat[0]), 1.0f/length(instanceData.modelMat[1]), 1.0f/length(instanceData.modelMat[2]));
		worldVertexPos = instanceData.modelMat * vec4(vertexIn + normalize(normalTmp)*(distance(camPos, viewProjectionMat *worldVertexPos))*0.002*scale, 1);
	}
	WorldPos = vec3(worldVertexPos);
	gl_Position = viewProjectionMat * worldVertexPos;
}
)");
	attachShader(vertexShader);

	std::shared_ptr<Shader> fragmentShader = std::make_shared<FragmentShader>();
	//language=glsl
	fragmentShader->setSource(R"(
#version 460 core
#extension GL_ARB_bindless_texture : require
out vec4 outputColor;

struct Material {

	//xy - metallicRoughness texture handle, z - metallic factor, w - roughness factor
	vec4 metallicRoughnessInfo;
	//xy - texture handle, zw - not used
	vec4 baseColorInfo;
	//xyzw - base color factor
	vec4 baseColorFactor;

	//xy - texture handle, z - scale, w - not used
	vec4 normalInfo;
	//xy - texture handle, z - strength, w - not used
	vec4 occlusionInfo;
	//xy - texture handle, zw - not used
	vec4 emissiveInfo;
	//xyz - emissive color, w - not used
	vec4 emissiveFactor;
};
in flat int instanceId;
in vec2 UVCoords;
in vec3 WorldPos;
in vec3 Normal;
in vec4 Color;
in flat uint hasColors;
in flat Material material;
uniform uint shaderSettings;

layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec4 camPos;
	int mode;
};

struct InstanceData {
	//Material material;
	mat4 modelMat;
	mat4 normalMat;
	int lightDataId;
};

layout(std430, binding = 1) buffer Instances {
	InstanceData instancesData[];
};

struct LightData{

	//xyz - color, w - type.
	vec4 colorAndType;
	float intensity;
	//for spot and point light
	float range;

	//for spotlight
	float innerConeAngle;
	float outerConeAngle;
};
layout(std430, binding = 3) buffer LightDatas {
	LightData lightsData[];
};

layout(std430, binding = 6) buffer LitBy {
	int lightInstanceIdsCount;
	int lightInstanceIds[];
};
const float PI = 3.14159265359;

vec3 getNormalFromMap(Material mat)
{
	vec3 normalIn ;
	uvec2 normalHandle = floatBitsToUint(mat.normalInfo.xy);
	if(normalHandle != uvec2(0.0f)){
		normalIn = texture(sampler2D(normalHandle), UVCoords).xyz;
		normalIn = normalize(normalIn * 2.0 - 1.0) * vec3(mat.normalInfo.z, mat.normalInfo.z, 1.0);
	}else{
		return Normal;
	}
	vec3 tangentNormal = normalIn * 2.0 - 1.0;

	vec3 Q1  = dFdx(WorldPos);
	vec3 Q2  = dFdy(WorldPos);
	vec2 st1 = dFdx(UVCoords);
	vec2 st2 = dFdy(UVCoords);

	vec3 N   = normalize(Normal);
	vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
	vec3 B  = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
#define COLORS_FLAG 4u


void main()
{
	vec3 albedo;
	uvec2 baseColorHandle = floatBitsToUint(material.baseColorInfo.xy);
	if(baseColorHandle != uvec2(0.0f)){
		albedo = pow(texture(sampler2D(baseColorHandle), UVCoords).rgb, vec3(2.2));
	}else {
		albedo = vec3(0.3f);
	}
	albedo *= material.baseColorFactor.xyz;

	float metallic;
	float roughness;
	uvec2 mrHandle = floatBitsToUint(material.metallicRoughnessInfo.xy);
	if(mrHandle != uvec2(0.0f)){
		vec4 val = texture(sampler2D(mrHandle), UVCoords);
		metallic = val.b ;
		roughness = val.g;
	}else{
		metallic = 1.0f;
		roughness =  1.0f;
	}
	metallic *= material.metallicRoughnessInfo.z;
	roughness *= material.metallicRoughnessInfo.w;

	float ao;
	uvec2 occlusionHandle = floatBitsToUint(material.occlusionInfo.xy);
	if(occlusionHandle != uvec2(0.0f)){
		ao = texture(sampler2D(occlusionHandle), UVCoords).r;
		ao=1.0f+material.occlusionInfo.z*(ao-1.0f);
	}else{
		ao = 1.0f;
	}


	vec3 N = getNormalFromMap(material);
	vec3 V = normalize(camPos.xyz - WorldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < lightInstanceIdsCount; ++i)
	{
		int lightInstance = lightInstanceIds[i];
		InstanceData instData = instancesData[lightInstance];
		LightData light = lightsData[instData.lightDataId];

		// calculate per-light radiance
		vec3 lightPos = instData.modelMat[3].xyz;
		vec3 lightDir;
		if(light.colorAndType.w == 0){//directional
			lightDir = normalize(-vec3(instData.modelMat[0][2], instData.modelMat[1][2], instData.modelMat[2][2]));
		}else
			lightDir = normalize(lightPos.xyz - WorldPos);
		vec3 H = normalize(V + lightDir);

		float distance = length(lightPos.xyz - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = light.colorAndType.xyz*light.intensity;


		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);

		float NdotV = max(dot(N, V), 0.0);
		float NdotL = max(dot(N, lightDir), 0.0);
		float HdotV = max(dot(H, V), 0.0);
		float ggx2 = GeometrySchlickGGX(NdotV, roughness);
		float ggx1 = GeometrySchlickGGX(NdotL, roughness);

		float G   = ggx1 * ggx2;
		vec3 F    = fresnelSchlick(HdotV, F0);
		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * NdotV * NdotL + 0.0001; // + 0.0001 to prevent divide by zero
		vec3 specular = numerator / denominator;

		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - metallic;

		// scale light by NdotL

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = (ambient + Lo);

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2));

	outputColor = vec4(color, 1.0);

	if(hasColors==1)
		outputColor*=Color;
	if(mode == 1) outputColor = vec4(1.0f);
})");
	attachShader(fragmentShader);
}

std::shared_ptr<DefaultProgram> DefaultProgram::getInstance() { return instance; }
} // namespace ke