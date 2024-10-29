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
// Created by alexus on 30.09.24.
//

#include "PresenterResourceSelection.h"

#include "EngineSDK/main/resources/IResourceLoadExecutor.h"
#include "EngineSDK/main/resources/ResourceLoadResult.h"
#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "EngineSDK/main/resources/models/IModel3DResource.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionProperty.h"
#include "IModelResourceSelection.h"
#include "IViewResourceSelection.h"
#include "mvp/ApplicationController.h"
#include "project/LoadedScene.h"

namespace mer::editor::mvp {
PresenterResourceSelection::PresenterResourceSelection(const std::shared_ptr<IModelResourceSelection> &pModel,
													   const std::shared_ptr<IViewResourceSelection> &pView)
	: model(pModel), view(pView) {

	model->setPresenter(this);
	view->setPresenter(this);
	if (auto property = dynamic_cast<sdk::main::ExtensionProperty<std::shared_ptr<sdk::main::IModel3DObject>>*>(
			model->getPropertyBase())) {
		auto type = model->getElementType();
		if (type == ExplorerElementType::NONE) model->setElementType(ExplorerElementType::RESOURCE_MODEL_OBJECT);
		auto value = property->getValue();
		if (value) { view->selectResourceByUri(value->getIModelResource()->asResource()->getResourceUri()); }
	}
}

void PresenterResourceSelection::selectClicked() {

	if (model->getElementType() == ExplorerElementType::RESOURCE_MODEL_OBJECT) {
		std::shared_ptr<sdk::main::IModel3DObject> object;
		if (auto modelResource = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(model->getSelectedResource())) {
			auto objectName = view->getSelectedObject();
			if (objectName.empty()) return;
			object = modelResource->getModelObject(objectName);
		}
		if (!object) return;
		if (auto property = dynamic_cast<sdk::main::ExtensionProperty<std::shared_ptr<sdk::main::IModel3DObject>>*>(
				model->getPropertyBase())) {
			property->setValue(object);
			getAppController()->stop(this);
		}
	}
}

void PresenterResourceSelection::onCancelClicked() { getAppController()->stop(this); }

void PresenterResourceSelection::unsetClicked() {
	auto propertyBase = model->getPropertyBase();
	if (auto property =
			dynamic_cast<sdk::main::ExtensionProperty<std::shared_ptr<sdk::main::IModel3DObject>>*>(propertyBase)) {
		property->setValue(nullptr);
		getAppController()->stop(this);
	}
}

void PresenterResourceSelection::onElementSelectionChanged(ProjectExplorerElement* pElement) {
	view->setSelectButtonSensitivity(false);
	view->setVisibleNoObjectsMessage(true);
	view->displayObjects({});

	if (model->getElementType() == ExplorerElementType::RESOURCE_MODEL_OBJECT) {
		if (pElement->getType() != ExplorerElementType::RESOURCE_MODEL) { return; }
		auto loadedScene = model->getLoadedScene();
		auto executor = loadedScene->getResourceLoadExecutor();
		if (!executor) return;
		executor->loadResourceAsync(
			pElement->getRelativePath(), [this](const std::shared_ptr<sdk::main::ResourceLoadResult> &pResult) {
				//TODO Display error
				if (pResult->isErrored()) {
					sdk::utils::Logger::error(pResult->getError());
					return;
				}
				if (!pResult->isReady()) return;
				auto resourceResult = pResult->getResource();
				model->setSelectedResource(resourceResult);

				auto property = dynamic_cast<sdk::main::ExtensionProperty<std::shared_ptr<sdk::main::IModel3DObject>>*>(
					model->getPropertyBase());
				auto &currentObject = property->getValue();

				bool hasCurrentObject{};
				if (auto modelResource = std::dynamic_pointer_cast<sdk::main::IModel3DResource>(resourceResult)) {
					std::vector<std::string> objects;
					for (auto [name, object]: modelResource->getModelObjects()) {
						if (currentObject && object == currentObject)
							//
							hasCurrentObject = true;
						objects.push_back(name);
					}
					view->displayObjects(objects);
					if (hasCurrentObject) view->selectObject(currentObject->getName());

					view->setVisibleNoObjectsMessage(false);
				}
			});
	}
}

void PresenterResourceSelection::onObjectSelectionChanged(const std::string & /*pObjectName*/) {
	view->setSelectButtonSensitivity(true);
}

void PresenterResourceSelection::run() { view->openView(); }

void PresenterResourceSelection::stop() { view->closeView(); }
} // namespace mer::editor::mvp