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
// Created by alexus on 29.03.24.
//

#ifndef MODELRENDEREXTENSION_H
#define MODELRENDEREXTENSION_H

#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Extension.h"

namespace mer::sdk::main {
class ShaderProgramRequest;
class ModelRequest;
class BuiltInProgramRequest;
class ModelResource;
} // namespace mer::sdk::main

namespace mer::sdk::renderer {
class ShaderProgram;
}

namespace mer::sdk::main {
class ModelRenderExtension : public Extension {

	uint32_t vao{};
	uint32_t vbo{};
	uint32_t ebo{};

protected:
	ModelRenderExtension() : propertyShader(this, "Shader"), propertyModel(this, "Model") {}

public:
	ExtensionProperty<std::shared_ptr<renderer::ShaderProgram>> propertyShader;
	ExtensionProperty<std::shared_ptr<ModelResource>> propertyModel;

	METHOD_CREATE(ModelRenderExtension)

	EXT_TYPE_NAME("ModelRenderExtension")

protected:

	utils::ReportMessagePtr onInit() override;

	utils::ReportMessagePtr onDeinit() override;

	void onRender() override;

private:
	void setupBuffers();
};
} // namespace mer::sdk::main


#endif //MODELRENDEREXTENSION_H
