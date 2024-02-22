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
