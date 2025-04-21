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

#ifndef LIGHT_H
#define LIGHT_H
#include <glm/vec4.hpp>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sigc++/signal.h>

namespace mer::sdk {

enum class LightType { DIRECTIONAL, POINT, SPOT };

NLOHMANN_JSON_SERIALIZE_ENUM(
	LightType, {{LightType::DIRECTIONAL, "directional"}, {LightType::POINT, "point"}, {LightType::SPOT, "spot"}})

struct LightData {
	//xyz - color, w - type.
	glm::vec4 colorAndType;
	float intensity;
	//for spot and point light
	float range;

	//for spotlight
	float innerConeAngle;
	float outerConeAngle;
};

class Light {
	std::string name;
	LightData data{};
	sigc::signal<void(Light* pSelf)> onLightChanged;

public:
	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] glm::vec3 getColor() const { return data.colorAndType; }

	void setColor(const glm::vec3 &pColor) {
		data.colorAndType.x = pColor.x;
		data.colorAndType.y = pColor.y;
		data.colorAndType.z = pColor.z;
		onLightChanged(this);
	}

	[[nodiscard]] LightType getType() const { return static_cast<LightType>(data.colorAndType.w); }

	void setType(LightType pType) {
		data.colorAndType.w = static_cast<float>(pType);
		onLightChanged(this);
	}

	[[nodiscard]] float getIntensity() const { return data.intensity; }

	void setIntensity(float pIntensity) {
		data.intensity = pIntensity;
		onLightChanged(this);
	}

	[[nodiscard]] float getRange() const { return data.range; }

	void setRange(float pRange) {
		data.range = pRange;
		onLightChanged(this);
	}

	[[nodiscard]] float getInnerConeAngle() const { return data.innerConeAngle; }

	void setInnerConeAngle(float pInnerConeAngle) {
		data.innerConeAngle = pInnerConeAngle;
		onLightChanged(this);
	}

	[[nodiscard]] float getOuterConeAngle() const { return data.outerConeAngle; }

	void setOuterConeAngle(float pOuterConeAngle) {
		data.outerConeAngle = pOuterConeAngle;
		onLightChanged(this);
	}

	[[nodiscard]] const LightData &getData() const { return data; }

	sigc::connection connectOnChanged(const sigc::slot<void(Light* pSelf)> &pSlot) {
		return onLightChanged.connect(pSlot);
	}

	void serialize(nlohmann::json &pJson);

	friend void to_json(nlohmann::json &pJson, const std::shared_ptr<Light> &pType) { pType->serialize(pJson); }

	void deserialize(const nlohmann::json &pJson);

	friend void from_json(const nlohmann::json &pJson, std::shared_ptr<Light> &pType) {
		pType = std::make_shared<Light>();
		pType->deserialize(pJson);
	}
};
} // namespace mer::sdk

#endif //LIGHT_H
