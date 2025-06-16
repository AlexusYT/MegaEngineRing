//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 24.02.25.
//

#include "KwasarEngine/gltf/Primitive.h"

#include "KwasarEngine/bounding/VolumeAabb.h"
#include "KwasarEngine/gltf/Accessor.h"
#include "KwasarEngine/gltf/Material.h"
#include "KwasarEngine/resources/shaders/ShaderProgram.h"
#include "GLTFSDK/GLTF.h"

namespace mer::sdk {
Primitive::Primitive(const MeshMode &pMeshMode, const std::shared_ptr<Accessor> &pPositionAccessor,
					 const std::shared_ptr<Accessor> &pIndexAccessor)
	: indexAccessor(pIndexAccessor), positionAccessor(pPositionAccessor), aabb(nullptr, "AABB") {
	mode = pMeshMode;
	if (positionAccessor) aabb = VolumeAabb::create(positionAccessor->getMinSize(), positionAccessor->getMaxSize());
	else
		aabb = VolumeAabb::create();
	//VolumeAabb test;
	//IPropertyNotifiable &t = dynamic_cast<IPropertyNotifiable &>(test);
}

std::shared_ptr<Primitive> Primitive::create(const MeshMode &pMeshMode,
											 const std::shared_ptr<Accessor> &pPositionAccessor,
											 const std::shared_ptr<Accessor> &pIndexAccessor) {
	return std::shared_ptr < Primitive > (new Primitive(pMeshMode, pPositionAccessor, pIndexAccessor));
}

void Primitive::setMaterial(const std::shared_ptr<Material> &pMaterial) {
	if (material == pMaterial) return;
	material = pMaterial;
	onMaterialChangedSignal(material);
}
} // namespace mer::sdk