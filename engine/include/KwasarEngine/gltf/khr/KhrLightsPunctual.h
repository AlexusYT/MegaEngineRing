//  KwasarEngine is an SDK that can help you speed up game development.
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

#ifndef KHRLIGHTSPUNCTUAL_H
#define KHRLIGHTSPUNCTUAL_H
#include <nlohmann/json_fwd.hpp>

#include "GLTFSDK/Extension.h"

namespace Microsoft::glTF {
class ExtensionDeserializer;
}

namespace ke {
class Light;

class KhrLightsPunctual : public Microsoft::glTF::Extension {
	std::vector<std::shared_ptr<Light>> lights;

public:
	class Node : public Extension {
	public:
		int32_t light{-1};

		std::unique_ptr<Extension> Clone() const override { return std::make_unique<Node>(*this); }

		std::string getName() const override { return "KHR_lights_punctual"; }

		bool IsEqual(const Extension &pRhs) const override {
			const auto other = dynamic_cast<const Node*>(&pRhs);

			return other != nullptr && this->light == other->light;
		}
	};

	static constexpr const char* EXTENSION_NAME = "KHR_lights_punctual";

	std::unique_ptr<Extension> Clone() const override { return std::make_unique<KhrLightsPunctual>(*this); }

	std::string getName() const override { return EXTENSION_NAME; }

	bool IsEqual(const Extension &pRhs) const override {
		const auto other = dynamic_cast<const KhrLightsPunctual*>(&pRhs);

		return other != nullptr && this->lights == other->lights;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Light>> &getLights() const { return lights; }

	static void addHandlers(Microsoft::glTF::ExtensionDeserializer &pDeserializer);

	static std::unique_ptr<Extension> deserializeDocument(
		const nlohmann::json &pJson,
		const std::shared_ptr<Microsoft::glTF::ExtensionDeserializer> & /*pExtensionDeserializer*/);

	static std::unique_ptr<Extension> deserializeNode(
		const nlohmann::json &pJson,
		const std::shared_ptr<Microsoft::glTF::ExtensionDeserializer> & /*pExtensionDeserializer*/);
};
} // namespace ke

#endif //KHRLIGHTSPUNCTUAL_H
