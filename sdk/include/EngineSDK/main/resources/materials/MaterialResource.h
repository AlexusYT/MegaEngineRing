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
// Created by alexus on 22.10.24.
//

#ifndef MATERIALRESOURCE_H
#define MATERIALRESOURCE_H

#include <glm/vec3.hpp>

#include "EngineSDK/main/Application.h"
#include "EngineSDK/main/resources/Resource.h"
#include "EngineSDK/main/resources/ResourceType.h"
#include "IMaterialResource.h"

namespace mer::sdk::main {
class ITextureResource;
}

namespace mer::sdk::main {

class MaterialResource : public IMaterialResource, public Resource {
	std::shared_ptr<ITextureResource> baseColorMap;
	std::shared_ptr<ITextureResource> normalMap;
	std::shared_ptr<ITextureResource> metallicMap;
	std::shared_ptr<ITextureResource> roughnessMap;
	std::shared_ptr<ITextureResource> aoMap;
	MaterialData data;

	sigc::signal<void()> onDataChangedSignal;

	MaterialResource();

public:
	~MaterialResource() override = default;

	static std::shared_ptr<MaterialResource> create();

	ResourceType getResourceType() override;

	void render() override;

	const MaterialData &getData() override { return data; }

	[[nodiscard]] const std::shared_ptr<ITextureResource> &getBaseColorMap() const override { return baseColorMap; }

	utils::ReportMessagePtr setBaseColorMap(const std::shared_ptr<ITextureResource> &pBaseColorMap) override;

	[[nodiscard]] std::optional<glm::vec3> getBaseColor() override;

	[[nodiscard]] const std::shared_ptr<ITextureResource> &getNormalMap() const override { return normalMap; }

	utils::ReportMessagePtr setNormalMap(const std::shared_ptr<ITextureResource> &pNormalMap) override;

	[[nodiscard]] std::optional<glm::vec3> getNormalColor() override;

	[[nodiscard]] const std::shared_ptr<ITextureResource> &getMetallicMap() const override { return metallicMap; }

	utils::ReportMessagePtr setMetallicMap(const std::shared_ptr<ITextureResource> &pMetallicMap) override;

	[[nodiscard]] std::optional<float> getMetallicColor() override;

	[[nodiscard]] const std::shared_ptr<ITextureResource> &getRoughnessMap() const override { return roughnessMap; }

	utils::ReportMessagePtr setRoughnessMap(const std::shared_ptr<ITextureResource> &pRoughnessMap) override;

	[[nodiscard]] std::optional<float> getRoughnessColor() override;

	[[nodiscard]] const std::shared_ptr<ITextureResource> &getAoMap() const override { return aoMap; }

	utils::ReportMessagePtr setAoMap(const std::shared_ptr<ITextureResource> &pAoMap) override;

	[[nodiscard]] std::optional<float> getAoColor() override;

	IResource* asResource() override { return this; }

	sigc::connection connectOnDataChangedSignal(const sigc::slot<void()> &pSlot) override {
		return onDataChangedSignal.connect(pSlot);
	}

private:
	static glm::vec4 handleToVec(uint64_t pHandle);
};


} // namespace mer::sdk::main

#endif //MATERIALRESOURCE_H
