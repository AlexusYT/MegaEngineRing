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

#ifndef SHADER_H
#define SHADER_H

#include <EngineSDK/main/resources/IResource.h>
#include <EngineSDK/renderer/shaders/IShader.h>

namespace mer::sdk::renderer {
enum class ShaderTypeEnum;
}

namespace mer::sdk::renderer {
class Shader : public IShader {
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
} // namespace mer::sdk::renderer
#endif //SHADER_H
