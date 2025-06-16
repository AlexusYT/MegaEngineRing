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
// Created by alexus on 04.02.24.
//

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glm/fwd.hpp>

#include "EngineSDK/resources/Resource.h"

namespace mer::sdk {
class Shader;

class ShaderProgram : public Resource {
	inline static const ShaderProgram* usedProgram{};
	uint32_t name;
	std::vector<std::shared_ptr<Shader>> attachedShaders;

public:
	ShaderProgram();

	~ShaderProgram() override;

	void attachShader(const std::shared_ptr<Shader> &pShader);

	void link() const;

	void validate() const;

	void use() const;

	void getInfoLog(std::string &pLogOut) const;

	[[nodiscard]] bool getDeleteStatus() const;

	[[nodiscard]] bool getLinkStatus() const;

	[[nodiscard]] bool getValidateStatus() const;

	[[nodiscard]] int getInfoLogLength() const;

	void setUniform(const std::string &pName, float pValue) const;

	void setUniform(const std::string &pName, int pValue) const;

	void setUniform(const std::string &pName, uint32_t pValue) const;

	void setUniform(const std::string &pName, const glm::vec3 &pValue) const;

	void setUniform(const std::string &pName, const glm::vec4 &pValue) const;

	void setUniform(const std::string &pName, const glm::mat4 &pValue) const;

	ResourceType getResourceType() override;

	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void addReportInfo(const ReportMessagePtr &pMsg) const override;
};
} // namespace mer::sdk


#endif //SHADERPROGRAM_H
