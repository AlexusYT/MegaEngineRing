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

#include "EngineSDK/main/prefabs/elements/MeshPrefabElement.h"

#include "EngineSDK/main/resources/models/Model3DObject.h"

namespace mer::sdk::main {
MeshPrefabElement::MeshPrefabElement(const std::shared_ptr<IModel3DObject> &pMesh, const std::string &pName)
	: PrefabElement(pName.empty() ? pMesh->getName() : pName), mesh(pMesh) {}

std::shared_ptr<MeshPrefabElement> MeshPrefabElement::create(const std::shared_ptr<IModel3DObject> &pMesh,
															 const std::string &pName) {
	return std::shared_ptr<MeshPrefabElement>(new MeshPrefabElement(pMesh, pName));
}

const std::vector<float> &MeshPrefabElement::getData() const { return mesh->getData(); }

const std::vector<uint16_t> &MeshPrefabElement::getIndices() const { return mesh->getIndices(); }
} // namespace mer::sdk::main