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
