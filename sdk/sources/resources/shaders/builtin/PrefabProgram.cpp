//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 03.12.24.
//

#include "EngineSDK/resources/shaders/builtin/PrefabProgram.h"

#include "EngineSDK/resources/shaders/Shader.h"

namespace mer::sdk {
std::shared_ptr<PrefabProgram> PrefabProgram::instance = std::shared_ptr<PrefabProgram>(new PrefabProgram);

PrefabProgram::PrefabProgram() {
	Resource::setResourceUri("_builtin_/shaders/PrefabProgram.enshader");

	std::shared_ptr<Shader> vertexShader = std::make_shared<VertexShader>();
	//language=glsl
	vertexShader->setSource(R"(
#version 460 core
layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec2 uvIn;
layout (location = 2) in vec3 normalsIn;

layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec3 camPos;
};

struct Material {
	vec4 baseColorMap;
	vec4 normalMap;
	vec4 metallicMap;
	vec4 roughnessMap;
	vec4 aoMap;
};

struct PrefabElementData {
	Material material;
	mat4 modelMat;
	mat4 normalMat;
	bool visible;
	bool _padding1;
	bool _padding2;
	bool _padding3;
};

uniform uint elementIdIn;
layout(std430, binding = 1) buffer PrefabElements {
	uint elementsPerInstance;
	PrefabElementData elementsData[];
};

struct InstanceData {
	bool visible;
	bool _padding;
};

layout(std430, binding = 2) buffer Instances {
	InstanceData instancesData[];
};
// lights
struct LightData{
	vec4 position;
	vec4 color;
	float power;
};
layout(std430, binding = 3) buffer Lights {
	LightData lightsData[];
};


out  InstanceInfo_VS_out {
	flat int instanceId;
	flat uint elementId;
} instanceInfo_VS;
out VertexInfo_VS_out{
	vec2 UVCoords;
	vec3 WorldPos;
	vec3 Normal;
} vertexInfo_VS;

void main(){
	instanceInfo_VS.instanceId=gl_InstanceID;
	InstanceData data = instancesData[instanceInfo_VS.instanceId];

	if(!data.visible) return;
	instanceInfo_VS.elementId = elementsPerInstance * instanceInfo_VS.instanceId + elementIdIn;
	PrefabElementData element = elementsData[instanceInfo_VS.elementId];

	if(!element.visible) return;
	vertexInfo_VS.UVCoords = uvIn;
	vec4 worldVertexPos = element.modelMat * vec4(vertexIn, 1);
	vertexInfo_VS.WorldPos = vec3(worldVertexPos);
	vertexInfo_VS.Normal = mat3(element.normalMat) * normalsIn;
	gl_Position = viewProjectionMat * worldVertexPos;
})");
	attachShader(vertexShader);

	std::shared_ptr<Shader> geometryShader = std::make_shared<GeometryShader>();
	//language=glsl
	geometryShader->setSource(R"(
#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in  InstanceInfo_VS_out{
	flat int instanceId;
	flat uint elementId;
} instanceInfo_GS_in[];

out InstanceInfo_GS_out{
	flat int instanceId;
	flat uint elementId;
} instanceInfo_GS_out;

in VertexInfo_VS_out{
	vec2 UVCoords;
	vec3 WorldPos;
	vec3 Normal;
} vertexInfo_GS_in[];

out VertexInfo_GS_out{
	vec2 UVCoords;
	vec3 WorldPos;
	vec3 Normal;
} vertexInfo_GS_out;

struct Material {
	vec4 baseColorMap;
	vec4 normalMap;
	vec4 metallicMap;
	vec4 roughnessMap;
	vec4 aoMap;
};
struct PrefabElementData{
	Material material;
	mat4 modelMat;
	mat4 normalMat;
	bool visible;
	bool _padding1;
	bool _padding2;
	bool _padding3;
};
layout(std430, binding = 1) buffer PrefabElements {
	uint elementsPerInstance;
	PrefabElementData elementsData[];
};

struct InstanceData {
	bool visible;
	bool _padding;
};

layout(std430, binding = 2) buffer Instances {
	InstanceData instancesData[];
};
void main() {

	if(!elementsData[instanceInfo_GS_in[0].elementId].visible || !instancesData[instanceInfo_GS_in[0].instanceId].visible) return;
	instanceInfo_GS_out.instanceId = instanceInfo_GS_in[0].instanceId;
	instanceInfo_GS_out.elementId = instanceInfo_GS_in[0].elementId;

	vertexInfo_GS_out.UVCoords = vertexInfo_GS_in[0].UVCoords;
	vertexInfo_GS_out.WorldPos = vertexInfo_GS_in[0].WorldPos;
	vertexInfo_GS_out.Normal = vertexInfo_GS_in[0].Normal;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();


	vertexInfo_GS_out.UVCoords = vertexInfo_GS_in[1].UVCoords;
	vertexInfo_GS_out.WorldPos = vertexInfo_GS_in[1].WorldPos;
	vertexInfo_GS_out.Normal = vertexInfo_GS_in[1].Normal;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

	vertexInfo_GS_out.UVCoords = vertexInfo_GS_in[2].UVCoords;
	vertexInfo_GS_out.WorldPos = vertexInfo_GS_in[2].WorldPos;
	vertexInfo_GS_out.Normal = vertexInfo_GS_in[2].Normal;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
)");
	attachShader(geometryShader);
	std::shared_ptr<Shader> fragmentShader = std::make_shared<FragmentShader>();
	//language=glsl
	fragmentShader->setSource(R"(
#version 460 core
#extension GL_ARB_bindless_texture : require
out vec4 outputColor;

in  InstanceInfo_GS_out{
	flat int instanceId;
	flat uint elementId;
} instanceInfo_FS_in;

in VertexInfo_GS_out{
	vec2 UVCoords;
	vec3 WorldPos;
	vec3 Normal;
} vertexInfo_FS_in;

layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec3 camPos;
};

struct Material {
	vec4 baseColorMap;
	vec4 normalMap;
	vec4 metallicMap;
	vec4 roughnessMap;
	vec4 aoMap;
};

struct PrefabElementData {
	Material material;
	mat4 modelMat;
	mat4 normalMat;
	bool visible;
	bool _padding1;
	bool _padding2;
	bool _padding3;
};

layout(std430, binding = 1) buffer PrefabElements {
	uint elementsPerInstance;
	PrefabElementData elementsData[];
};


struct LightData{
	vec4 position;
	vec4 color;
	float power;
};

// lights
layout(std430, binding = 3) buffer Lights {
	LightData lightsData[];
};

const float PI = 3.14159265359;

vec3 getNormalFromMap(Material material)
{
	vec3 normalIn ;
	if(material.normalMap.w == 2.0f){
		normalIn = texture(sampler2D(floatBitsToUint(material.normalMap.xy)), vertexInfo_FS_in.UVCoords).xyz;
	}else{
		normalIn = material.normalMap.xyz;
	}
	if(normalIn == vec3(0.0f, 0.0f, 0.0f)) return vertexInfo_FS_in.Normal;
	vec3 tangentNormal = normalIn * 2.0 - 1.0;

	vec3 Q1  = dFdx(vertexInfo_FS_in.WorldPos);
	vec3 Q2  = dFdy(vertexInfo_FS_in.WorldPos);
	vec2 st1 = dFdx(vertexInfo_FS_in.UVCoords);
	vec2 st2 = dFdy(vertexInfo_FS_in.UVCoords);

	vec3 N   = normalize(vertexInfo_FS_in.Normal);
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

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{

	PrefabElementData element = elementsData[instanceInfo_FS_in.elementId];
	Material material = element.material;
	vec3 albedo;
	if(material.baseColorMap.w == 2.0f){
		albedo = pow(texture(sampler2D(floatBitsToUint(material.baseColorMap.xy)), vertexInfo_FS_in.UVCoords).rgb, vec3(2.2));
	}else {
		albedo = material.baseColorMap.rgb;
	}

	float metallic;
	if(material.metallicMap.w == 2.0f){
		metallic = texture(sampler2D(floatBitsToUint(material.metallicMap.xy)), vertexInfo_FS_in.UVCoords).r;
	}else{
		metallic = material.metallicMap.r;
	}

	float roughness;
	if(material.roughnessMap.w == 2.0f){
		roughness = texture(sampler2D(floatBitsToUint(material.roughnessMap.xy)), vertexInfo_FS_in.UVCoords).r;
	}else{
		roughness = material.roughnessMap.r;
	}

	float ao;
	if(material.aoMap.w == 2.0f){
		ao = texture(sampler2D(floatBitsToUint(material.aoMap.xy)), vertexInfo_FS_in.UVCoords).r;
	}else{
		ao = material.aoMap.r;
	}

	vec3 N = getNormalFromMap(material);
	vec3 V = normalize(camPos - vertexInfo_FS_in.WorldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < lightsData.length(); ++i)
	{
		LightData light = lightsData[i];
		// calculate per-light radiance
		vec3 L = normalize(light.position.xyz - vertexInfo_FS_in.WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(light.position.xyz - vertexInfo_FS_in.WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = light.color.xyz*light.power * attenuation;

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(N, H, roughness);
		float G   = GeometrySmith(N, V, L, roughness);
		vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
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
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2));

	outputColor = vec4(color, 1.0);
})");
	attachShader(fragmentShader);
}

std::shared_ptr<PrefabProgram> PrefabProgram::getInstance() { return instance; }
} // namespace mer::sdk