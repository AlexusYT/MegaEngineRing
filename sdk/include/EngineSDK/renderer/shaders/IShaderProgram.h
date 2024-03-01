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

#ifndef ISHADERPROGRAM_H
#define ISHADERPROGRAM_H
#include <glm/fwd.hpp>

namespace mer::sdk::renderer {
class IShader;
class ISSBO;

class IShaderProgram {
public:
	virtual ~IShaderProgram() = default;

	virtual void attachShader(const std::shared_ptr<IShader> &pShader) = 0;

	virtual void attachSsbo(const std::shared_ptr<ISSBO> &pSsbo, const std::string &pBlockName, uint32_t pBinding) = 0;

	virtual void link() = 0;

	virtual void validate() const = 0;

	virtual void use() const = 0;

	virtual void getInfoLog(std::string &pLogOut) const = 0;

	[[nodiscard]] virtual bool getDeleteStatus() const = 0;

	[[nodiscard]] virtual bool getLinkStatus() const = 0;

	[[nodiscard]] virtual bool getValidateStatus() const = 0;

	[[nodiscard]] virtual int getInfoLogLength() const = 0;

	virtual void setUniform(const std::string &pName, float pValue) const = 0;

	virtual void setUniform(const std::string &pName, int pValue) const = 0;

	virtual void setUniform(const std::string &pName, const glm::vec3 &pValue) const = 0;

	virtual void setUniform(const std::string &pName, const glm::vec4 &pValue) const = 0;

	virtual void setUniform(const std::string &pName, const glm::mat4 &pValue) const = 0;
};
} // namespace mer::sdk::renderer


#endif //ISHADERPROGRAM_H
