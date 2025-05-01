//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 17.04.25.
//

#include "EngineSDK/gltf/Light.h"

#include <nlohmann/json.hpp>

namespace mer::sdk {
Light::Light(const std::string &pName) : name(pName) {
	data.colorAndType = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	data.intensity = 1.0f;
	data.innerConeAngle = 0.0f;
	data.outerConeAngle = M_PI_4f;
}

void Light::serialize(nlohmann::json &pJson) { pJson["name"] = name; }

void Light::deserialize(const nlohmann::json &pJson) {
	if (auto iter = pJson.find("name"); iter != pJson.end()) iter.value().get_to(name);
	else
		name.clear();
	glm::vec3 color(1.0f);
	if (auto iter = pJson.find("color"); iter != pJson.end()) {
		auto &colorArr = iter.value();
		for (int i = 0; auto &iterationProxyValue: colorArr) {
			try {
				iterationProxyValue.get_to(color[i]);
			} catch (...) { color[i] = 1.0f; }
			i++;
		}
	}
	setColor(color);

	if (auto iter = pJson.find("intensity"); iter != pJson.end()) setIntensity(iter.value().get<float>());
	else
		setIntensity(1.0f);

	setType(pJson.at("type").get<LightType>());

	if (auto iter = pJson.find("range"); iter != pJson.end()) setRange(iter.value().get<float>());
	else
		setRange(0.0f);

	if (getType() == LightType::SPOT) {
		float innerConeAngle = 0.0f;
		float outerConeAngle = M_PI_4f;
		if (auto iter = pJson.find("spot"); iter != pJson.end()) {
			auto &val = iter.value();
			if (auto innerIter = val.find("innerConeAngle"); innerIter != val.end())
				innerIter.value().get_to(innerConeAngle);
			if (auto innerIter = val.find("outerConeAngle"); innerIter != val.end())
				innerIter.value().get_to(outerConeAngle);
		}
		innerConeAngle = std::clamp(innerConeAngle, 0.0f, M_PI_2f);
		outerConeAngle = std::clamp(outerConeAngle, 0.0f, M_PI_2f);
		setInnerConeAngle(innerConeAngle);
		setOuterConeAngle(outerConeAngle);
	}
}
} // namespace mer::sdk