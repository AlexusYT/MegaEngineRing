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
#include <math.h>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sigc++/signal.h>

namespace ke {
enum class LightType { DIRECTIONAL, POINT, SPOT, LIGHT_TYPE_MAX };

inline const char* light_type_array_str[3] = {"directional", "point", "spot"};

inline const char* to_string(const LightType pE) {
	if (pE < LightType::DIRECTIONAL || pE > LightType::LIGHT_TYPE_MAX) return "unknown";
	return light_type_array_str[static_cast<size_t>(pE)];
}

NLOHMANN_JSON_SERIALIZE_ENUM(LightType, {{LightType::DIRECTIONAL, to_string(LightType::DIRECTIONAL)},
							 {LightType::POINT, to_string(LightType::POINT)},
							 {LightType::SPOT, to_string(LightType::SPOT)}})

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

protected:
	explicit Light(const std::string &pName);

public:
	static std::shared_ptr<Light> create(const std::string &pName = "Unnamed") {
		return std::shared_ptr<Light>(new Light(pName));
	}

	/**
	 * @brief Clone this instance. You can set a new name to the new instance.
	 * @param pNewName Name for new instance. Default is "Unnamed Copy".
	 * @return Duplicated instance.
	 */
	std::shared_ptr<Light> duplicate(const std::string &pNewName = "Unnamed Copy") const {
		auto newLight = create(pNewName);
		newLight->data = data;
		return newLight;
	}

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
		pInnerConeAngle = std::clamp(pInnerConeAngle, 0.0f, data.outerConeAngle);
		if (data.innerConeAngle == pInnerConeAngle) return;
		data.innerConeAngle = pInnerConeAngle;
		onLightChanged(this);
	}

	[[nodiscard]] float getOuterConeAngle() const { return data.outerConeAngle; }

	void setOuterConeAngle(float pOuterConeAngle) {
		pOuterConeAngle = std::clamp(pOuterConeAngle, data.innerConeAngle, std::numbers::pi_v<float> / 2.0f);
		if (data.outerConeAngle == pOuterConeAngle) return;
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
		pType = create();
		pType->deserialize(pJson);
	}
};
} // namespace ke

#endif //LIGHT_H
