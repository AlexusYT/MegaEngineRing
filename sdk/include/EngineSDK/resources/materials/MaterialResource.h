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
// Created by alexus on 22.10.24.
//

#ifndef MATERIALRESOURCE_H
#define MATERIALRESOURCE_H

#include <atomic>
#include <sigc++/scoped_connection.h>
#include <sigc++/signal.h>

#include "EngineSDK/resources/Resource.h"
#include "EngineUtils/utils/Property.h"
#include "IMaterialResource.h"

namespace mer::sdk {
class IMaterialComponent;
class ITextureResource;
} // namespace mer::sdk

namespace mer::sdk {
class MaterialResource : public IMaterialResource, public Resource {
	Property<std::shared_ptr<IMaterialComponent>> baseColorMap;
	sigc::scoped_connection baseColorMapConnection;
	Property<std::shared_ptr<IMaterialComponent>> normalMap;
	sigc::scoped_connection normalMapConnection;
	Property<std::shared_ptr<IMaterialComponent>> metallicMap;
	sigc::scoped_connection metallicMapConnection;
	Property<std::shared_ptr<IMaterialComponent>> roughnessMap;
	sigc::scoped_connection roughnessMapConnection;
	Property<std::shared_ptr<IMaterialComponent>> aoMap;
	sigc::scoped_connection aoMapConnection;
	MaterialData data;
	std::atomic<bool> dirty{false};

	sigc::signal<void(const MaterialData &pData)> onDataChangedSignal;

	MaterialResource();

public:
	static std::shared_ptr<MaterialResource> defaultMaterial;

	~MaterialResource() override = default;

	static std::shared_ptr<MaterialResource> create();

	ResourceType getResourceType() override;

	void render() override;

	const MaterialData &getData() override { return data; }

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getAlbedo() override {
		return baseColorMap.getReadOnly();
	}

	void setAlbedo(const std::shared_ptr<IMaterialComponent> &pBaseColorMap) override;

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getNormal() override {
		return normalMap.getReadOnly();
	}

	void setNormal(const std::shared_ptr<IMaterialComponent> &pNormalMap) override;

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getMetallic() override {
		return metallicMap.getReadOnly();
	}

	void setMetallic(const std::shared_ptr<IMaterialComponent> &pMetallicMap) override;

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getRoughness() override {
		return roughnessMap.getReadOnly();
	}

	void setRoughness(const std::shared_ptr<IMaterialComponent> &pRoughnessMap) override;

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<IMaterialComponent>> getAo() override { return aoMap.getReadOnly(); }

	void setAo(const std::shared_ptr<IMaterialComponent> &pAoMap) override;

	IResource* asResource() override { return this; }

	sigc::connection connectOnDataChangedSignal(const sigc::slot<void(const MaterialData &pData)> &pSlot) override {
		pSlot(data);
		return onDataChangedSignal.connect(pSlot);
	}

private:
	sigc::scoped_connection connectComponentValChanged(const std::shared_ptr<IMaterialComponent> &pComponent,
													   glm::vec4* pMap);
};
} // namespace mer::sdk

#endif //MATERIALRESOURCE_H
