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
// Created by alexus on 31.01.24.
//


#include "EngineSDK/resources/shaders/Shader.h"

#include <epoxy/gl.h>

#include "EngineSDK/utils/ReportMessage.h"

namespace mer::sdk {
Shader::Shader(const ShaderTypeEnum pType) : type(pType) {}

Shader::~Shader() { Shader::uninitialize(); }

void Shader::setSource(const std::string &pSrc) { source = pSrc; }

void Shader::getSource(std::string &pSrcOut) const {
	if (!source.empty()) {
		pSrcOut = source;
		return;
	}
	const int sourceLen = getSourceLength();
	pSrcOut.resize(static_cast<size_t>(sourceLen));
	glGetShaderSource(name, sourceLen, nullptr, pSrcOut.data());
}

void Shader::getInfoLog(std::string &pLogOut) const {
	const int logLen = getInfoLogLength();
	pLogOut.resize(static_cast<size_t>(logLen));
	glGetShaderInfoLog(name, logLen, nullptr, pLogOut.data());
}

void Shader::compile() const {
	glCompileShader(name);
}

ShaderTypeEnum Shader::getShaderType() const {
	switch (getNativeShaderType()) {
		case 0x91B9: return ShaderTypeEnum::COMPUTE_SHADER;
		case 0x8B31: return ShaderTypeEnum::VERTEX_SHADER;
		case 0x8E88: return ShaderTypeEnum::TESS_CONTROL_SHADER;
		case 0x8E87: return ShaderTypeEnum::TESS_EVALUATION_SHADER;
		case 0x8DD9: return ShaderTypeEnum::GEOMETRY_SHADER;
		case 0x8B30: return ShaderTypeEnum::FRAGMENT_SHADER;
		default: return ShaderTypeEnum::MAX_SHADER_TYPE;
	}
}

int Shader::getNativeShaderType() const {
	int out;
	glGetShaderiv(name, GL_SHADER_TYPE, &out);
	return out;
}

bool Shader::getDeleteStatus() const {
	int out;
	glGetShaderiv(name, GL_DELETE_STATUS, &out);
	return out;
}

bool Shader::getCompileStatus() const {
	int out;
	glGetShaderiv(name, GL_COMPILE_STATUS, &out);
	return out;
}

int Shader::getInfoLogLength() const {
	int out;
	glGetShaderiv(name, GL_INFO_LOG_LENGTH, &out);
	return out;
}

int Shader::getSourceLength() const {
	int out;
	glGetShaderiv(name, GL_SHADER_SOURCE_LENGTH, &out);
	return out;
}

ReportMessagePtr Shader::onInitialize() {
	uint32_t glType;
	switch (type) {
		case ShaderTypeEnum::COMPUTE_SHADER: glType = 0x91B9;
			break;
		case ShaderTypeEnum::VERTEX_SHADER: glType = 0x8B31;
			break;
		case ShaderTypeEnum::TESS_CONTROL_SHADER: glType = 0x8E88;
			break;
		case ShaderTypeEnum::TESS_EVALUATION_SHADER: glType = 0x8E87;
			break;
		case ShaderTypeEnum::GEOMETRY_SHADER: glType = 0x8DD9;
			break;
		case ShaderTypeEnum::FRAGMENT_SHADER: glType = 0x8B30;
			break;
		default: glType = 0;
	}
	name = glCreateShader(glType);
	const int len = static_cast<int>(source.length());
	const char* srcTemp = source.c_str();
	glShaderSource(name, 1, &srcTemp, &len);
	//source.clear();
	compile();
	if (!getCompileStatus()) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to compile the shader");
		msg->setMessage("Error in shader code detected");
		addReportInfo(msg);
		std::string log;
		getInfoLog(log);
		msg->addInfoLine("Compilation log: \n{}", log);
		return msg;
	}
	return nullptr;
}

void Shader::onUninitialize() {
	if (name) {
		glDeleteShader(name);
		name = 0;
	}
}

void Shader::addReportInfo(const ReportMessagePtr &pMsg) const {
	pMsg->addInfoLine("Shader type: {}", to_string(type));
	pMsg->addInfoLine("Is compiled: {}", getCompileStatus());
}
} // namespace mer::sdk
