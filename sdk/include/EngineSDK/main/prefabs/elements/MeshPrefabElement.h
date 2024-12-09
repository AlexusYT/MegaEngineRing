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

#ifndef MESHPREFABELEMENT_H
#define MESHPREFABELEMENT_H
#include <memory>

#include "EngineSDK/main/resources/models/IModel3DObject.h"
#include "PrefabElement.h"

namespace mer::sdk::main {
class IModel3DObject;

class MeshPrefabElement : public PrefabElement {
	std::shared_ptr<IModel3DObject> mesh;

protected:
	explicit MeshPrefabElement(const std::shared_ptr<IModel3DObject> &pMesh, const std::string &pName = "");

public:
	static std::shared_ptr<MeshPrefabElement> create(const std::shared_ptr<IModel3DObject> &pMesh,
													 const std::string &pName = "");

	const std::vector<float> &getData() const override;

	const std::vector<uint16_t> &getIndices() const override;
};

} // namespace mer::sdk::main

#endif //MESHPREFABELEMENT_H
