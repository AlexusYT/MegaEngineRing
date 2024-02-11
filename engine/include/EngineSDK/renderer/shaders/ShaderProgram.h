//
// Created by alexus on 04.02.24.
//

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <EngineSDK/main/resources/IResource.h>

#include "EngineSDK/renderer/shaders/IShaderProgram.h"

namespace n::sdk::renderer {

class ShaderProgram : public IShaderProgram, public main::IResource {
	uint32_t name;
	std::vector<std::shared_ptr<IShader>> attachedShaders;
	std::vector<std::shared_ptr<ISSBO>> buffers;

public:
	ShaderProgram();

	~ShaderProgram() override;

	void attachShader(const std::shared_ptr<IShader> &pShader) override;

	void attachSsbo(const std::shared_ptr<ISSBO> &pSsbo, const std::string &pBlockName, uint32_t pBinding) override;

	void link() override;

	void validate() const override;

	void use() const override;

	void getInfoLog(std::string &pLogOut) const override;

	[[nodiscard]] bool getDeleteStatus() const override;

	[[nodiscard]] bool getLinkStatus() const override;

	[[nodiscard]] bool getValidateStatus() const override;

	[[nodiscard]] int getInfoLogLength() const override;

	void setUniform(const std::string &pName, float pValue) const override;

	void setUniform(const std::string &pName, int pValue) const override;

	void setUniform(const std::string &pName, const glm::vec3 &pValue) const override;

	void setUniform(const std::string &pName, const glm::vec4 &pValue) const override;

	void setUniform(const std::string &pName, const glm::mat4 &pValue) const override;
};


} // namespace n::sdk::renderer


#endif //SHADERPROGRAM_H
