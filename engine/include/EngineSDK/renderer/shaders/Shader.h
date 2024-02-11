//
// Created by alexus on 31.01.24.
//

#ifndef SHADER_H
#define SHADER_H

#include <EngineSDK/main/resources/IResource.h>
#include <EngineSDK/renderer/shaders/IShader.h>

namespace n::sdk::renderer {
enum class ShaderTypeEnum;
}

namespace n::sdk::renderer {
class Shader : public IShader, public main::IResource {
	uint32_t name;

protected:
	explicit Shader(ShaderTypeEnum pType);

public:
	~Shader() override;

	void setSource(const std::string &pSrc) const override;

	void getSource(std::string &pSrcOut) const override;

	void getInfoLog(std::string &pLogOut) const override;

	void compile() const override;

	ShaderTypeEnum getShaderType() const override;

	int getNativeShaderType() const override;

	bool getDeleteStatus() const override;

	bool getCompileStatus() const override;

	int getInfoLogLength() const override;

	int getSourceLength() const override;

	uint32_t native() const override { return name; }
};
} // namespace n::sdk::renderer
#endif //SHADER_H
