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
// Created by alexus on 27.11.24.
//

#include "KwasarEngine/prefabs/elements/PrefabElement.h"

#include "KwasarEngine/prefabs/Prefab.h"
#include "KwasarEngine/prefabs/elements/PrefabElementInstance.h"
#include "KwasarEngine/resources/materials/IMaterialResource.h"
#include "KwasarEngine/utils/UUID.h"

namespace ke {
PrefabElement::PrefabElement(const std::string &pName)
	: uuid(UUID::newInstance()), name(pName), visible(nullptr, ""), material(nullptr, "") {
	visible = true;
	material.connectEvent([this](const std::shared_ptr<IMaterialResource> &pMaterial) {
		for (auto instance: instances) { instance->onParentMaterialChanged(pMaterial); }
	});
}

void PrefabElement::update() { if (prefab) prefab->updateElement(this); }

std::shared_ptr<PrefabElementInstance> PrefabElement::createInstance() {
	auto instance = PrefabElementInstance::create();
	addInstance(instance);
	return instance;
}

void PrefabElement::addInstance(std::shared_ptr<PrefabElementInstance> pInstance) {
	instances.emplace_back(pInstance);
	if (const auto interface = std::dynamic_pointer_cast<IPrefabElementInstance>(pInstance))
		interface->setElement(this);
}

void PrefabElement::setIndicesStart(uint64_t pIndicesStart) { indicesStart = pIndicesStart; }

void PrefabElement::setDataStart(uint64_t pDataStart) { dataStart = pDataStart; }

void PrefabElement::setIndicesCount(uint64_t pIndicesCount) { indicesCount = pIndicesCount; }

void PrefabElement::setDataCount(uint64_t pDataCount) { dataCount = pDataCount; }

void PrefabElement::onParentMaterialChanged(const std::shared_ptr<IMaterialResource> &pMaterial) {
	if (*material) return;
	for (auto instance: instances) { instance->onParentMaterialChanged(pMaterial); }
}
} // namespace ke
