//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

#include "LightExtension.h"

#include <glm/vec4.hpp>

#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "MainObjectExtension.h"

namespace mer::sdk::main {
LightExtension::LightExtension() : propertyPower(this, "Power"), propertyColor(this, "Light color") {
	propertyPower.getEvent().connect([this](const float &pNewVal) {
		data.power = pNewVal;
		notifyDataChanged();
	});
	propertyPower.setValue(10.0f);
	propertyColor.getEvent().connect([this](const glm::vec3 &pNewVal) {
		data.color = glm::vec4(pNewVal, 1.0f);
		notifyDataChanged();
	});
	propertyColor.setValue(glm::vec3(1.0f, 1.0f, 1.0f));
}

utils::ReportMessagePtr LightExtension::onInit() {
	auto mainExt = getObject()->getMainExtension();
	mainExt->propertyPosition.connectEvent([this](const glm::vec3 &pPos) {
		data.position = glm::vec4(pPos, 1.0f);
		notifyDataChanged();
	});
	return nullptr;
}
} // namespace mer::sdk::main