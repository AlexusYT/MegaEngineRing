//
// Created by alexus on 31.01.24.
//

#ifndef ISHADER_H
#define ISHADER_H
#include <EngineSDK/renderer/shaders/ShaderTypeEnum.h>
#include <cstdint>

namespace mer::sdk::renderer {
class IShader {

protected:
	IShader() = default;

public:
	virtual ~IShader() = default;

	virtual void setSource(const std::string &pSrc) const = 0;

	virtual void getSource(std::string &pSrcOut) const = 0;

	virtual void getInfoLog(std::string &pLogOut) const = 0;

	virtual void compile() const = 0;

	virtual ShaderTypeEnum getShaderType() const = 0;

	virtual int getNativeShaderType() const = 0;

	virtual bool getDeleteStatus() const = 0;

	virtual bool getCompileStatus() const = 0;

	virtual int getInfoLogLength() const = 0;

	virtual int getSourceLength() const = 0;

	virtual uint32_t native() const = 0;
};
} // namespace mer::sdk::renderer


#endif //ISHADER_H
