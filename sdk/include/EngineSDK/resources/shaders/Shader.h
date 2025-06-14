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

#ifndef SHADER_H
#define SHADER_H


#include "EngineSDK/render/Initializable.h"
#include "EngineUtils/utils/IReportable.h"
#include "ShaderTypeEnum.h"

namespace mer::sdk {
enum class ShaderTypeEnum;

class Shader : public Initializable, public IReportable {
	uint32_t name;
	ShaderTypeEnum type;
	std::string source;

protected:
	explicit Shader(ShaderTypeEnum pType);

public:
	~Shader() override;

	void setSource(const std::string &pSrc);

	void getSource(std::string &pSrcOut) const;

	void getInfoLog(std::string &pLogOut) const;

	void compile() const;

	ShaderTypeEnum getShaderType() const;

	int getNativeShaderType() const;

	bool getDeleteStatus() const;

	bool getCompileStatus() const;

	int getInfoLogLength() const;

	int getSourceLength() const;

	uint32_t native() const { return name; }

	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

	void addReportInfo(const ReportMessagePtr &pMsg) const override;
};

class FragmentShader : public Shader {
public:
	FragmentShader() : Shader(ShaderTypeEnum::FRAGMENT_SHADER) {}
};

class GeometryShader : public Shader {
public:
	explicit GeometryShader() : Shader(ShaderTypeEnum::GEOMETRY_SHADER) {}
};

class VertexShader : public Shader {
public:
	VertexShader() : Shader(ShaderTypeEnum::VERTEX_SHADER) {}
};
} // namespace mer::sdk
#endif //SHADER_H
