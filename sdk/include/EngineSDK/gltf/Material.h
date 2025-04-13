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
// Created by alexus on 26.02.25.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/vec4.hpp>
#include <sigc++/connection.h>
#include <sigc++/functors/slot.h>
#include <sigc++/signal.h>
#include <unordered_map>

#include "EngineSDK/render/Initializable.h"

namespace Microsoft::glTF {
enum class TextureType;
struct Material;
} // namespace Microsoft::glTF

namespace mer::sdk {
class Texture;

struct MaterialData {

	//xy - metallicRoughness texture handle, z - metallic factor, w - roughness factor
	glm::vec4 metallicRoughnessInfo{};
	//xy - texture handle, zw - not used
	glm::vec4 baseColorInfo{};
	//xyzw - base color factor
	glm::vec4 baseColorFactor{};

	//xy - texture handle, z - scale, w - not used
	glm::vec4 normalInfo{};
	//xy - texture handle, z - strength, w - not used
	glm::vec4 occlusionInfo{};
	//xy - texture handle, zw - not used
	glm::vec4 emissiveInfo{};
	//xyz - emissive color, w - not used
	glm::vec4 emissiveFactor{};
};

class Material : public Initializable {
	std::string name;
	MaterialData data{};
	int alphaMode;
	float alphaCutoff;
	bool doubleSided;
	sigc::signal<void(Material* pSelf)> onMaterialChanged;
	std::unordered_map<Microsoft::glTF::TextureType, std::shared_ptr<Texture>> usedTextures;

	Material(const Microsoft::glTF::Material &pMaterial,
			 const std::unordered_map<Microsoft::glTF::TextureType, std::shared_ptr<Texture>> &pUsedTextures);

public:
	static std::shared_ptr<Material> create(
		const Microsoft::glTF::Material &pMaterial,
		const std::unordered_map<Microsoft::glTF::TextureType, std::shared_ptr<Texture>> &pUsedTextures);

	[[nodiscard]] const MaterialData &getData() const { return data; }

	sigc::connection connectOnChanged(const sigc::slot<void(Material* pSelf)> &pSlot) {
		return onMaterialChanged.connect(pSlot);
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

protected:
	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;

private:
	static glm::vec2 handleToVec(const uint64_t pHandle);
};

} // namespace mer::sdk

#endif //MATERIAL_H
