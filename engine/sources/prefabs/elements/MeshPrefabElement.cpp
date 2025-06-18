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

#include "KwasarEngine/prefabs/elements/MeshPrefabElement.h"

#include "KwasarEngine/resources/models/Model3DObject.h"

namespace ke {
MeshPrefabElement::MeshPrefabElement() : PrefabElement("MeshElement") {}

std::shared_ptr<MeshPrefabElement> MeshPrefabElement::create() {
	return std::shared_ptr < MeshPrefabElement > (new MeshPrefabElement());
}

const std::vector<float> &MeshPrefabElement::getData() const {
	if (!mesh) return defaultVertices;
	return mesh->getData();
}

const std::vector<uint16_t> &MeshPrefabElement::getIndices() const {
	if (!mesh) return defaultIndices;
	return mesh->getIndices();
}
} // namespace ke