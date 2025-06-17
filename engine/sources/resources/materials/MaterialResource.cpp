//  KwasarEngine is an SDK that can help you speed up game development.
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


#include "KwasarEngine/resources/materials/MaterialResource.h"

#include "KwasarEngine/resources/ResourceType.h"
#include "KwasarEngine/resources/materials/IMaterialComponent.h"

namespace ke {
std::shared_ptr<MaterialResource> MaterialResource::defaultMaterial = nullptr;

MaterialResource::MaterialResource()
	: baseColorMap(this, "Albedo map (base color)"),
	  normalMap(this, "Normal map"),
	  metallicMap(this, "Metallic map"),
	  roughnessMap(this, "Roughness Map"),
	  aoMap(this, "Ambient occlusion map") {
	data.aoMap = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	data.metallicMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	data.roughnessMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	data.normalMap = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	data.baseColorMap = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

std::shared_ptr<MaterialResource> MaterialResource::create() {
	return std::shared_ptr < MaterialResource > (new MaterialResource());
}

ResourceType MaterialResource::getResourceType() { return ResourceType::MATERIAL; }

void MaterialResource::render() {
	if (dirty) {
		onDataChangedSignal(data);
		dirty.store(false);
	}
}

void MaterialResource::setAlbedo(const std::shared_ptr<IMaterialComponent> &pBaseColorMap) {
	if (baseColorMap == pBaseColorMap) return;
	baseColorMap = pBaseColorMap;
	if (pBaseColorMap) {
		baseColorMapConnection = connectComponentValChanged(pBaseColorMap, &data.baseColorMap);
	} else {
		if (baseColorMapConnection) baseColorMapConnection.disconnect();
		data.baseColorMap = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		dirty = true;
	}
}

void MaterialResource::setNormal(const std::shared_ptr<IMaterialComponent> &pNormalMap) {
	if (normalMap == pNormalMap) return;
	normalMap = pNormalMap;
	if (pNormalMap) { normalMapConnection = connectComponentValChanged(pNormalMap, &data.normalMap); } else {
		if (normalMapConnection) normalMapConnection.disconnect();
		data.normalMap = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		dirty = true;
	}
}

void MaterialResource::setMetallic(const std::shared_ptr<IMaterialComponent> &pMetallicMap) {
	if (metallicMap == pMetallicMap) return;
	metallicMap = pMetallicMap;
	if (pMetallicMap) { metallicMapConnection = connectComponentValChanged(pMetallicMap, &data.metallicMap); } else {
		if (metallicMapConnection) metallicMapConnection.disconnect();
		data.metallicMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		dirty = true;
	}
}

void MaterialResource::setRoughness(const std::shared_ptr<IMaterialComponent> &pRoughnessMap) {
	if (roughnessMap == pRoughnessMap) return;
	roughnessMap = pRoughnessMap;
	if (pRoughnessMap) {
		roughnessMapConnection = connectComponentValChanged(pRoughnessMap, &data.roughnessMap);
	} else {
		if (roughnessMapConnection) roughnessMapConnection.disconnect();
		data.roughnessMap = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		dirty = true;
	}
}

void MaterialResource::setAo(const std::shared_ptr<IMaterialComponent> &pAoMap) {
	if (aoMap == pAoMap) return;
	aoMap = pAoMap;
	if (pAoMap) { aoMapConnection = connectComponentValChanged(pAoMap, &data.aoMap); } else {
		if (aoMapConnection) aoMapConnection.disconnect();
		data.aoMap = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		dirty = true;
	}
}

sigc::scoped_connection MaterialResource::connectComponentValChanged(
	const std::shared_ptr<IMaterialComponent> &pComponent, glm::vec4* pMap) {
	return sigc::scoped_connection(
		pComponent->getComponentValueProperty().connectEvent([this, pMap](const glm::vec4 &pValue) {
			*pMap = pValue;
			dirty.store(true);
		}));
}
} // namespace ke