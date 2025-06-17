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
// Created by alexus on 27.11.24.
//


#include "KwasarEngine/prefabs/PrefabInstance.h"

#include "KwasarEngine/prefabs/elements/PrefabElementInstance.h"
#include "KwasarEngine/resources/materials/IMaterialResource.h"
#include "KwasarEngine/utils/Transformation.h"

namespace ke {
PrefabInstance::PrefabInstance()
	: data(nullptr, ""), visible(nullptr, ""), material(nullptr, "") {
	visible = true;
	visible.connectEvent([this](const bool &pVisible) {
		data->visible = pVisible;
		data.notifyChanged();
	});

	material.connectEvent([this](const std::shared_ptr<IMaterialResource> &pMaterial) {
		for (auto element: elements) { element.second->onParentMaterialChanged(pMaterial); }
	});
}

std::shared_ptr<PrefabInstance> PrefabInstance::create() {
	return std::shared_ptr < PrefabInstance > (new PrefabInstance());
}

std::shared_ptr<PrefabElementInstance> PrefabInstance::getElement(const UUID &pUuid) const {
	auto iter = elements.find(pUuid);
	if (iter == elements.end()) return nullptr;
	return iter->second;
}

void PrefabInstance::addElement(const UUID &pUuid, const std::shared_ptr<PrefabElementInstance> &pInstance) {
	pInstance->setTransformParent(this);
	elements.emplace(pUuid, pInstance);
}

void PrefabInstance::removeElement(const UUID &pUuid) {
	auto iter = elements.find(pUuid);
	if (iter == elements.end()) return;
	iter->second->setTransformParent(nullptr);
	elements.erase(iter);
}

void PrefabInstance::onGlobalTransformChanged(const std::shared_ptr<Transformation> & /*pTransformation*/) {
	for (auto element: elements) { element.second->updateTransform(); }
}
} // namespace ke