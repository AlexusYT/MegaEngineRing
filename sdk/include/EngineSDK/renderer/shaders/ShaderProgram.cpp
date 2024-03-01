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

#include "ShaderProgram.h"

#include <epoxy/gl.h>
#include <glm/mat4x4.hpp>

#include "EngineSDK/renderer/buffers/ISSBO.h"
#include "IShader.h"

namespace mer::sdk::renderer {

ShaderProgram::ShaderProgram() { name = glCreateProgram(); }

ShaderProgram::~ShaderProgram() { glDeleteProgram(name); }

void ShaderProgram::attachShader(const std::shared_ptr<IShader> &pShader) {
	attachedShaders.emplace_back(pShader);
	glAttachShader(name, pShader->native());
}

void ShaderProgram::attachSsbo(const std::shared_ptr<ISSBO> &pSsbo, const std::string &pBlockName,
							   const uint32_t pBinding) {
	buffers.emplace_back(pSsbo);
	const uint32_t index = glGetProgramResourceIndex(name, 0x92E6, pBlockName.c_str());
	glShaderStorageBlockBinding(name, index, pBinding);
	pSsbo->bindBufferBase(pBinding);
}

void ShaderProgram::link() { glLinkProgram(name); }

void ShaderProgram::validate() const { glValidateProgram(name); }

void ShaderProgram::use() const {
	//if (this == usedProgram) return;
	glUseProgram(name);
	//usedProgram = this;
}

void ShaderProgram::getInfoLog(std::string &pLogOut) const {

	const int logLen = getInfoLogLength();
	pLogOut.resize(static_cast<size_t>(logLen));
	glGetProgramInfoLog(name, logLen, nullptr, pLogOut.data());
}

bool ShaderProgram::getDeleteStatus() const {
	int out;
	glGetProgramiv(name, GL_DELETE_STATUS, &out);
	return out;
}

bool ShaderProgram::getLinkStatus() const {
	int out;
	glGetProgramiv(name, GL_LINK_STATUS, &out);
	return out;
}

bool ShaderProgram::getValidateStatus() const {
	int out;
	glGetProgramiv(name, GL_VALIDATE_STATUS, &out);
	return out;
}

int ShaderProgram::getInfoLogLength() const {
	int out;
	glGetProgramiv(name, GL_INFO_LOG_LENGTH, &out);
	return out;
}

void ShaderProgram::setUniform(const std::string &pName, const float pValue) const {
	glUniform1f(glGetUniformLocation(name, pName.c_str()), pValue);
}

void ShaderProgram::setUniform(const std::string &pName, const int pValue) const {
	glUniform1i(glGetUniformLocation(name, pName.c_str()), pValue);
}

void ShaderProgram::setUniform(const std::string &pName, const glm::vec3 &pValue) const {
	glUniform3fv(glGetUniformLocation(name, pName.c_str()), 1, &pValue[0]);
}

void ShaderProgram::setUniform(const std::string &pName, const glm::vec4 &pValue) const {
	glUniform4fv(glGetUniformLocation(name, pName.c_str()), 1, &pValue[0]);
}

void ShaderProgram::setUniform(const std::string &pName, const glm::mat4 &pValue) const {
	glUniformMatrix4fv(glGetUniformLocation(name, pName.c_str()), 1, GL_FALSE, &pValue[0][0]);
}
} // namespace mer::sdk::renderer