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
// Created by alexus on 01.10.24.
//

#include "EngineSDK/main/resources/models/Model3DResource.h"

#include "EngineSDK/main/resources/models/IModel3DObject.h"

namespace mer::sdk::main {

std::shared_ptr<Model3DResource> Model3DResource::create() {
	return std::shared_ptr<Model3DResource>(new Model3DResource());
}

utils::ReportMessagePtr Model3DResource::onInitialize() {
	for (auto &[name, object]: modelObjects) { object->initialize(); }
	return nullptr;
}

void Model3DResource::render() {
	for (auto &[name, object]: modelObjects) { object->render(); }
}

void Model3DResource::onUninitialize() {
	for (auto &[name, object]: modelObjects) { object->uninitialize(); }
}

void Model3DResource::addModelObject(const std::shared_ptr<IModel3DObject> &pObject) {
	pObject->setIModelResource(this);
	modelObjects.emplace(pObject->getName(), pObject);
}

void Model3DResource::removeModelObject(const std::shared_ptr<IModel3DObject> &pObject) {
	pObject->setIModelResource(nullptr);
	modelObjects.erase(pObject->getName());
}

void Model3DResource::removeModelObject(const std::string &pName) { removeModelObject(modelObjects.at(pName)); }

std::shared_ptr<IModel3DObject> Model3DResource::getModelObject(const std::string &pName) {
	const auto iter = modelObjects.find(pName);
	if (iter == modelObjects.end()) return nullptr;
	return iter->second;
}
} // namespace mer::sdk::main