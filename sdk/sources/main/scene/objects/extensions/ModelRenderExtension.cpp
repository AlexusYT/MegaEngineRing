// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 29.03.24.
//

#include "EngineSDK/main/scene/objects/extensions/ModelRenderExtension.h"

#include <glm/ext/matrix_transform.hpp>

#include "EngineSDK/main/resources/materials/IMaterialResource.h"
#include "EngineSDK/main/resources/materials/MaterialResource.h"
#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/MainObjectExtension.h"

namespace mer::sdk::main {

utils::ReportMessagePtr ModelRenderExtension::onInit() {
	auto &pos = getObject()->getMainExtension()->propertyPosition;

	pos.connectEvent([this](const glm::vec3 &pPos) {
		data.modelViewMatrix = glm::translate(glm::mat4(1.0f), pPos + glm::vec3(0.0f, 1, 0));
		data.normalMatrix = glm::transpose(glm::inverse(glm::mat3(data.modelViewMatrix)));
		notifyDataChanged();
	});


	if (propertyModel.getValue()) propertyModel->addRenderInstance(this);

	propertyModel.getReturnOverride().connect([this](const std::shared_ptr<IModel3DObject> &pObj) {
		if (const auto val = propertyModel.getValue()) val->removeRenderInstance(this);
		return pObj;
	});
	propertyModel.getEvent().connect([this](const std::shared_ptr<IModel3DObject> &pObj) {
		if (!pObj) return;
		pObj->addRenderInstance(this);
	});
	propertyMaterial.connectEvent([this](const std::shared_ptr<IMaterialResource> &pMaterial) {
		if (pMaterial) {
			pMaterial->connectOnDataChangedSignal([this](const MaterialData &pData) {
				data.material = pData;
				notifyDataChanged();
			});
		} else {
			data.material = MaterialResource::defaultMaterial->getData();
		}
		notifyDataChanged();
	});
	return nullptr;
}

utils::ReportMessagePtr ModelRenderExtension::onDeinit() { return nullptr; }

void ModelRenderExtension::onRender() {}


} // namespace mer::sdk::main