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
// Created by alexus on 31.01.24.
//

#include "Shader.h"

#include <epoxy/gl.h>
#include "ShaderTypeEnum.h"

namespace mer::sdk::renderer {
Shader::Shader(const ShaderTypeEnum pType) {
	uint32_t type;
	switch (pType) {

		case ShaderTypeEnum::COMPUTE_SHADER: type = 0x91B9; break;
		case ShaderTypeEnum::VERTEX_SHADER: type = 0x8B31; break;
		case ShaderTypeEnum::TESS_CONTROL_SHADER: type = 0x8E88; break;
		case ShaderTypeEnum::TESS_EVALUATION_SHADER: type = 0x8E87; break;
		case ShaderTypeEnum::GEOMETRY_SHADER: type = 0x8DD9; break;
		case ShaderTypeEnum::FRAGMENT_SHADER: type = 0x8B30; break;
		default: type = 0;
	}
	name = glCreateShader(type);
}

Shader::~Shader() { glDeleteShader(name); }

void Shader::setSource(const std::string &pSrc) const {
	const int len = static_cast<int>(pSrc.length());
	const char* srcTemp = pSrc.c_str();
	glShaderSource(name, 1, &srcTemp, &len);
}

void Shader::getSource(std::string &srcOut) const {
	const int sourceLen = getSourceLength();
	srcOut.resize(static_cast<size_t>(sourceLen));
	glGetShaderSource(name, sourceLen, nullptr, srcOut.data());
}

void Shader::getInfoLog(std::string &logOut) const {
	const int logLen = getInfoLogLength();
	logOut.resize(static_cast<size_t>(logLen));
	glGetShaderInfoLog(name, logLen, nullptr, logOut.data());
}

void Shader::compile() const { glCompileShader(name); }

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
} // namespace mer::sdk::renderer