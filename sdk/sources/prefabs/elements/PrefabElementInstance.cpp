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


#include "EngineSDK/prefabs/elements/PrefabElementInstance.h"

#include <glm/ext/matrix_transform.hpp>

#include "EngineSDK/resources/materials/MaterialResource.h"
#include "EngineSDK/utils/Transformation.h"

namespace mer::sdk::main {
PrefabElementInstance::PrefabElementInstance() : data(nullptr, ""), visible(nullptr, ""), material(nullptr, "") {

	visible = true;
	visible.connectEvent([this](const bool &pVisible) {
		data->visible = pVisible;
		data.notifyChanged();
	});
	material.connectEvent([this](const std::shared_ptr<IMaterialResource> &pPropertyMaterial) {
		if (pPropertyMaterial) {
			materialConnection = pPropertyMaterial->connectOnDataChangedSignal(
				sigc::mem_fun(*this, &PrefabElementInstance::onMaterialChanged));
		} else {
			onMaterialChanged(MaterialResource::defaultMaterial->getData());
		}
	});
}

std::shared_ptr<PrefabElementInstance> PrefabElementInstance::create() {
	return std::shared_ptr<PrefabElementInstance>(new PrefabElementInstance());
}

void PrefabElementInstance::onParentMaterialChanged(const std::shared_ptr<IMaterialResource> &pMaterial) {
	if (*material) return;
	if (pMaterial) {
		materialConnection =
			pMaterial->connectOnDataChangedSignal(sigc::mem_fun(*this, &PrefabElementInstance::onMaterialChanged));
	} else {
		onMaterialChanged(MaterialResource::defaultMaterial->getData());
	}
}

void PrefabElementInstance::onGlobalTransformChanged(const std::shared_ptr<utils::Transformation> &pTransformation) {
	data->modelMat = pTransformation->getModelMatrix();
	data->normalMat = glm::transpose(glm::inverse(glm::mat3(data->modelMat)));
	data.notifyChanged();
}

void PrefabElementInstance::onMaterialChanged(const MaterialData &pData) {
	data->material = pData;
	data.notifyChanged();
}

} // namespace mer::sdk::main