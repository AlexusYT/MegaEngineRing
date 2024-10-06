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

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "EngineSDK/main/resources/Resource.h"
#include "EngineSDK/renderer/shaders/IShaderProgram.h"

namespace mer::sdk::renderer {

class ShaderProgram : public IShaderProgram, public main::Resource {
	inline static const ShaderProgram* usedProgram{};
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


} // namespace mer::sdk::renderer


#endif //SHADERPROGRAM_H
