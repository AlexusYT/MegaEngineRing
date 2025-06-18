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
// Created by alexus on 06.12.24.
//


#include "KwasarEngine/prefabs/PrefabSsbo.h"

#include <cstring>
#include <sigc++/scoped_connection.h>

#include "KwasarEngine/prefabs/PrefabInstance.h"

namespace ke {
PrefabSsbo::PrefabSsbo() {
	SSBO::setData(instancesData.data(), static_cast<int64_t>(instancesData.size()), STREAM_DRAW);
}

std::shared_ptr<PrefabSsbo> PrefabSsbo::create() { return std::shared_ptr < PrefabSsbo > (new PrefabSsbo()); }

void PrefabSsbo::trackInstance(PrefabInstance* pInstance) {
	auto dataProperty = pInstance->getData();
	instances.emplace(pInstance,
					  dataProperty.connectEvent([this](const PrefabInstanceData & /*pNewData*/) { onDataChanged(); }));
	onDataChanged();
}

void PrefabSsbo::untrackInstance(PrefabInstance* pInstance) {
	instances.erase(pInstance);
	onDataChanged();
}

void PrefabSsbo::render() {
	if (!dirty) return;

	if (getSize() < dataSize) { reallocate(dataSize, ssboData); } else
		bufferSubData(0, dataSize, ssboData);
	dirty = false;
}

void PrefabSsbo::onDataChanged() {
	instancesData.clear();
	instancesData.resize(instances.size());
	for (auto &instance: instances) {
		const auto instanceId = instance.first->getInstanceId();

		if (instancesData.size() <= instanceId) { instancesData.resize(instanceId + 1); }
		instancesData.at(instanceId) = instance.first->getData();
	}

	auto newSize = static_cast<int64_t>(instancesData.size() * sizeof(PrefabInstanceData));
	if (newSize != dataSize) {
		ssboData = realloc(ssboData, static_cast<size_t>(newSize));
		dataSize = newSize;
	}
	memcpy(ssboData, instancesData.data(), instancesData.size() * sizeof(PrefabInstanceData));

	dirty = true;
}
} // namespace ke