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
#include "EngineSDK/main/render/RenderInstance.h"
#include "EngineSDK/main/render/RenderInstanceData.h"
#include "Extension.h"

namespace mer::sdk::main {
class IMaterialResource;
class IModel3DObject;

class ModelRenderExtension : public Extension, public RenderInstance {

	RenderInstanceData data;

protected:
	ModelRenderExtension() : propertyModel(this, "Model"), propertyMaterial(this, "Material") {}

public:
	ExtensionProperty<std::shared_ptr<IModel3DObject>> propertyModel;
	ExtensionProperty<std::shared_ptr<IMaterialResource>> propertyMaterial;

	METHOD_CREATE(ModelRenderExtension)

	EXT_TYPE_NAME("ModelRenderExtension")

	const RenderInstanceData &getRenderInstanceData() override { return data; }

	std::optional<MaterialData> getMaterialData() override;

	std::shared_ptr<renderer::ShaderProgram> getShader() override { return nullptr; }

protected:
	utils::ReportMessagePtr onInit() override;

	utils::ReportMessagePtr onDeinit() override;

	void onRender() override;
};
} // namespace mer::sdk::main


#endif //MODELRENDEREXTENSION_H
