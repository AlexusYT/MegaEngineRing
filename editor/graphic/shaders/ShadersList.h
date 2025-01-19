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
// Created by alexus on 06.12.23.
//

#ifndef SHADERSLIST_H
#define SHADERSLIST_H
#if 0

	#include <Render/opengl/shaders/ShaderFwd.h>
	#include <Render/opengl/shaders/ShaderProgramFwd.h>

class ShadersList {
	static std::map<std::string, rel::opengl::ShaderUPtr> vertexShaders;
	static std::map<std::string, rel::opengl::ShaderUPtr> geometryShaders;
	static std::map<std::string, rel::opengl::ShaderUPtr> fragmentShaders;
	static std::map<std::string, rel::opengl::ShaderProgramUPtr> shaderPrograms;

public:
	static bool addVertexShader(const std::filesystem::path &file, const std::string &name);
	static bool addGeometryShader(const std::filesystem::path &file, const std::string &name);
	static bool addFragmentShader(const std::filesystem::path &file, const std::string &name);

	/*static bool addShaderProgram(const std::string &name, const std::string &vertexShaderName,
								 const std::string &fragmentShaderName, const std::string &geometryShaderName) {
		return addShaderProgram(name, {vertexShaderName}, {fragmentShaderName}, {geometryShaderName});
	}*/
	static bool addShaderProgram(const std::string &name, const std::string &vertexShaderName,
								 const std::string &fragmentShaderName) {
		return addShaderProgram(name, {vertexShaderName}, {fragmentShaderName}, {});
	}

	static bool addShaderProgram(const std::string &name, const std::vector<std::string> &vertexShadersName,
								 const std::vector<std::string> &fragmentShadersName,
								 const std::vector<std::string> &geometryShadersName = {});

	static rel::opengl::ShaderProgram* getProgram(const std::string &name) {
		const auto iter = shaderPrograms.find(name);
		if (iter == shaderPrograms.end()) return nullptr;
		return iter->second.get();
	}

	static void clear();

private:
	static rel::opengl::Shader* getVertexShader(const std::string &name) {
		const auto iter = vertexShaders.find(name);
		if (iter == vertexShaders.end()) return nullptr;
		return iter->second.get();
	}

	static rel::opengl::Shader* getGeometryShader(const std::string &name) {
		const auto iter = geometryShaders.find(name);
		if (iter == geometryShaders.end()) return nullptr;
		return iter->second.get();
	}

	static rel::opengl::Shader* getFragmentShader(const std::string &name) {
		const auto iter = fragmentShaders.find(name);
		if (iter == fragmentShaders.end()) return nullptr;
		return iter->second.get();
	}

	static bool readFromFile(const std::filesystem::path &file, std::string &out);

	static bool createShader(const std::filesystem::path &file, rel::opengl::ShaderUPtr &shader);
};
#endif


#endif //SHADERSLIST_H
