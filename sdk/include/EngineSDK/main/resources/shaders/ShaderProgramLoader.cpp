// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 04.02.24.
//

#include "ShaderProgramLoader.h"

#include "EngineSDK/renderer/shaders/FragmentShader.h"
#include "EngineSDK/renderer/shaders/ShaderProgram.h"
#include "EngineSDK/renderer/shaders/VertexShader.h"
#include "EngineUtils/utils/ReportMessage.h"

namespace mer::sdk::main {

utils::ReportMessagePtr ShaderProgramLoader::load(IResourceLoadExecutor* /*pLoadExecutor*/,
												  std::shared_ptr<std::istream> &pStream,
												  std::shared_ptr<IResource> &pResourceOut) {
	std::shared_ptr<renderer::ShaderProgram> program = std::make_shared<renderer::ShaderProgram>();
	program->setResourceUri(readString(pStream));
	uint8_t shadersCount;
	pStream->read(reinterpret_cast<char*>(&shadersCount), sizeof(uint8_t));
	for (uint8_t i = 0; i < shadersCount; i++) {
		std::shared_ptr<renderer::Shader> shader;
		uint8_t shaderType;
		pStream->read(reinterpret_cast<char*>(&shaderType), sizeof(uint8_t));
		if (shaderType == 0) { //vertex shader
			shader = std::make_shared<renderer::VertexShader>();
		} else if (shaderType == 1) { //fragment shader
			shader = std::make_shared<renderer::FragmentShader>();
		} else
			continue;
		if (std::dynamic_pointer_cast<renderer::FragmentShader>(shader)) {
			//language=glsl
			shader->setSource(R"(#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_NV_gpu_shader5 : require
out vec4 outputColor;
in flat int instanceId;
in vec2 UVCoords;
in vec3 WorldPos;
in vec3 Normal;

layout(std430, binding = 0) buffer ProgramWideSettings {
	mat4 viewProjectionMat;
	mat4 projectionMat;
	mat4 viewMat;
	vec3 camPos;
};

struct Material {
	sampler2D colorMapUint;
	vec4 baseColorMap;
	vec4 normalMap;
	vec4 metallicMap;
	vec4 roughnessMap;
	vec4 aoMap;
};

// material parameters
layout(std430, binding = 2) buffer Materials {
	Material materialsData[];
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
		normalIn = texture(sampler2D(floatBitsToUint(material.normalMap.xy)), UVCoords).xyz;
	}else{
		normalIn = material.normalMap.xyz;
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

	Material material = materialsData[instanceId];
	vec3 albedo;
//000A0000 01000000
//00000001 00000A00
		uint64_t i = 0x0000000100000A00UL;
	if(material.baseColorMap.w == 2.0f){
		uvec2 convertedHandle;
		convertedHandle.y = 0x00000001U;//uint(i & uint64_t(0xFFFFFFFFL));
		convertedHandle.x = 0x00000A00U;//uint((i >> 32) & uint64_t(0xFFFFFFFFL));
		//convertedHandle.y = 0x000A0000U;//uint(i & uint64_t(0xFFFFFFFFL));
		//convertedHandle.x = 0x01000000U;//uint((i >> 32) & uint64_t(0xFFFFFFFFL));
		outputColor = vec4(texture(material.colorMapUint, UVCoords).rgb, 1);
		return;
		albedo = pow(texture(sampler2D(floatBitsToUint(material.baseColorMap.xy)), UVCoords).rgb, vec3(2.2));
	}else {
		outputColor = vec4(1);
		return;
		albedo = material.baseColorMap.rgb;
	}

	float metallic;
	if(material.metallicMap.w == 2.0f){
		metallic = texture(sampler2D(floatBitsToUint(material.metallicMap.xy)), UVCoords).r;
	}else{
		metallic = material.metallicMap.r;
	}

	float roughness;
	if(material.roughnessMap.w == 2.0f){
		roughness = texture(sampler2D(floatBitsToUint(material.roughnessMap.xy)), UVCoords).r;
	}else{
		roughness = material.roughnessMap.r;
	}

	float ao;
	if(material.aoMap.w == 2.0f){
		ao = texture(sampler2D(floatBitsToUint(material.aoMap.xy)), UVCoords).r;
	}else{
		ao = material.aoMap.r;
	}

	vec3 N = getNormalFromMap(material);
	vec3 V = normalize(camPos - WorldPos);

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
		vec3 L = normalize(light.position.xyz - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(light.position.xyz - WorldPos);
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

	// ambient lighting (note that the next IBL tutorial will replace
	// this ambient lighting with environment lighting).
	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0/2.2));

	outputColor = vec4(color, 1.0);
})");
		} else
			shader->setSource(readString(pStream));
		shader->compile();
		if (!shader->getCompileStatus()) {
			auto msg = utils::ReportMessage::create();
			msg->setTitle("Failed to compile the shader");
			msg->setMessage("Error in shader code detected");
			msg->addInfoLine("Shader type: {}", shaderType == 0 ? "vertex" : "fragment");
			std::string log;
			shader->getInfoLog(log);
			msg->addInfoLine("Compilation log: {}", log);
			return msg;
		}
		program->attachShader(shader);
	}
	program->link();
	if (!program->getLinkStatus()) {

		auto msg = utils::ReportMessage::create();
		msg->setStacktrace();
		msg->setTitle("Failed to link the shader program");
		msg->setMessage("Error in shader code detected");
		std::string log;
		program->getInfoLog(log);
		msg->addInfoLine("Linker log: {}", log);
		return msg;
	}
	pResourceOut = program;
	return nullptr;
}

utils::ReportMessagePtr ShaderProgramLoader::init(IResourceLoadExecutor* /*pLoadExecutor*/,
												  const std::shared_ptr<IResource> & /*pLoadedResource*/) {
	return nullptr;
}
} // namespace mer::sdk::main