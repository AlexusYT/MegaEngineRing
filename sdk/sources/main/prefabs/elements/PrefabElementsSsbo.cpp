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
// Created by alexus on 27.11.24.
//

#include "EngineSDK/main/prefabs/elements/PrefabElementsSsbo.h"

#include <cstring>

#include "EngineSDK/main/prefabs/Prefab.h"
#include "EngineSDK/main/prefabs/PrefabInstance.h"
#include "EngineSDK/main/prefabs/elements/PrefabElement.h"
#include "EngineSDK/main/prefabs/elements/PrefabElementInstance.h"

namespace mer::sdk::main {
PrefabElementsSsbo::PrefabElementsSsbo() {
	SSBO::setData(instancesData.data(), static_cast<int64_t>(instancesData.size()), renderer::STREAM_DRAW);
}

PrefabElementsSsbo::~PrefabElementsSsbo() {
	if (ssboData) free(ssboData);
}

std::shared_ptr<PrefabElementsSsbo> PrefabElementsSsbo::create() {
	return std::shared_ptr<PrefabElementsSsbo>(new PrefabElementsSsbo());
}

void PrefabElementsSsbo::trackInstance(PrefabElementInstance* pInstance) {
	auto dataProperty = pInstance->getData();
	instances.emplace(pInstance, dataProperty.connectEvent(
									 [this](const PrefabElementInstanceData & /*pNewData*/) { onDataChanged(); }));
	onDataChanged();
}

void PrefabElementsSsbo::untrackInstance(PrefabElementInstance* pInstance) {
	instances.erase(pInstance);
	onDataChanged();
}

void PrefabElementsSsbo::untrackAll() {
	instances.clear();
	instancesData.clear();
	dirty = true;
}

void PrefabElementsSsbo::render() {
	if (!dirty) return;

	if (getSize() < dataSize) {
		reallocate(dataSize, ssboData);
	} else
		bufferSubData(0, dataSize, ssboData);
	dirty = false;
}

void PrefabElementsSsbo::onDataChanged() {
	instancesData.clear();
	instancesData.resize(instances.size());
	for (auto &instance: instances) {
		auto element = instance.first->getElement();
		auto prefabInstance = instance.first->getPrefabInstance();
		const auto instanceId = prefabInstance->getInstanceId();

		const auto id = elementsPerInstance * instanceId + element->getIdInPrefab();
		if (instancesData.size() <= id) { instancesData.resize(id + 1); }
		instancesData.at(id) = instance.first->getData();
	}

	auto elementsCountSize = sizeof(elementsPerInstance) * 4;
	auto newSize = static_cast<int64_t>(instancesData.size() * sizeof(PrefabElementInstanceData) + elementsCountSize);
	if (newSize != dataSize) {
		ssboData = realloc(ssboData, static_cast<size_t>(newSize));
		dataSize = newSize;
	}
	memcpy(ssboData, &elementsPerInstance, elementsCountSize);
	memcpy(static_cast<char*>(ssboData) + elementsCountSize, instancesData.data(),
		   instancesData.size() * sizeof(PrefabElementInstanceData));

	dirty = true;
}
} // namespace mer::sdk::main