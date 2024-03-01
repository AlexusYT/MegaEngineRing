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
// Created by alexus on 06.12.23.
//

#include "ShadersList.h"
#if 0

	#include <Render/opengl/shaders/Shader.h>
	#include <Render/opengl/shaders/ShaderProgram.h>


std::map<std::string, rel::opengl::ShaderUPtr> ShadersList::vertexShaders;
std::map<std::string, rel::opengl::ShaderUPtr> ShadersList::geometryShaders;
std::map<std::string, rel::opengl::ShaderUPtr> ShadersList::fragmentShaders;
std::map<std::string, rel::opengl::ShaderProgramUPtr> ShadersList::shaderPrograms;

bool ShadersList::addVertexShader(const std::filesystem::path &file, const std::string &name) {
	rel::opengl::ShaderUPtr shader = rel::opengl::Shader::create(rel::opengl::VERTEX_SHADER);
	if (!createShader(file, shader)) return false;
	vertexShaders.emplace(name, std::move(shader));
	return true;
}

bool ShadersList::addGeometryShader(const std::filesystem::path &file, const std::string &name) {
	rel::opengl::ShaderUPtr shader = rel::opengl::Shader::create(rel::opengl::GEOMETRY_SHADER);
	if (!createShader(file, shader)) return false;
	geometryShaders.emplace(name, std::move(shader));
	return true;
}

bool ShadersList::addFragmentShader(const std::filesystem::path &file, const std::string &name) {
	rel::opengl::ShaderUPtr shader = rel::opengl::Shader::create(rel::opengl::FRAGMENT_SHADER);
	if (!createShader(file, shader)) return false;
	fragmentShaders.emplace(name, std::move(shader));
	return true;
}

bool ShadersList::addShaderProgram(const std::string &name, const std::vector<std::string> &vertexShadersName,
								   const std::vector<std::string> &fragmentShadersName,
								   const std::vector<std::string> &geometryShadersName) {
	rel::opengl::ShaderProgramUPtr shaderProgram = rel::opengl::ShaderProgram::create();
	for (const auto &shaderName: vertexShadersName) {
		if (auto* shader = getVertexShader(shaderName)) shaderProgram->attachShader(shader);
		else {
			std::cerr << "Vertex shader " << shaderName << " not found\n";
			return false;
		}
	}

	for (const auto &shaderName: geometryShadersName) {
		if (auto* shader = getVertexShader(shaderName)) shaderProgram->attachShader(shader);
		else {
			std::cerr << "Geometry shader " << shaderName << " not found\n";
			return false;
		}
	}

	for (const auto &shaderName: fragmentShadersName) {
		if (auto* shader = getVertexShader(shaderName)) shaderProgram->attachShader(shader);
		else {
			std::cerr << "Fragment shader " << shaderName << " not found\n";
			return false;
		}
	}

	shaderProgram->link();

	if (!shaderProgram->getLinkStatus()) {
		std::cerr << "Failed to link shader program " << name << ": " << shaderProgram->getInfoLog() << std::endl;
		return false;
	}

	shaderPrograms.emplace(name, std::move(shaderProgram));
	return true;
}

void ShadersList::clear() {
	shaderPrograms.clear();
	vertexShaders.clear();
	geometryShaders.clear();
	fragmentShaders.clear();
}

bool ShadersList::readFromFile(const std::filesystem::path &file, std::string &out) {
	std::ifstream stream(file);
	if (!stream.is_open()) {
		std::cerr << "Failed to open shader file: " << file << std::endl;
		return false;
	}
	std::stringstream ss;
	ss << stream.rdbuf();
	out = ss.str();
	return true;
}

bool ShadersList::createShader(const std::filesystem::path &file, rel::opengl::ShaderUPtr &shader) {
	std::string code;
	if (!readFromFile(absolute("Resources/Shaders" / file), code)) return false;
	shader->setSource(code);
	shader->compile();
	if (!shader->getCompileStatus()) {
		std::cerr << "Failed to compile shader " << file << ": " << shader->getInfoLog() << std::endl;
		return false;
	}
	return true;
}
#endif
