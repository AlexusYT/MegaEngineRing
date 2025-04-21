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
// Created by alexus on 23.10.24.
//

#ifndef LIGHTEXTENSION_H
#define LIGHTEXTENSION_H
#include "EngineSDK/light/LightInstance1.h"
#include "EngineSDK/light/LightInstanceData.h"
#include "Extension.h"

namespace mer::sdk {

class LightExtension : public Extension, public LightInstance1 {
	LightInstanceData data;

public:
	ExtensionProperty<float> propertyPower;
	ExtensionProperty<glm::vec3> propertyColor;

	LightExtension();

	METHOD_CREATE(LightExtension)

	EXT_TYPE_NAME("LightExtension")

private:
	[[nodiscard]] const LightInstanceData &getLightInstanceData() const override { return data; }

	ReportMessagePtr onInit() override;
};

} // namespace mer::sdk

#endif //LIGHTEXTENSION_H
