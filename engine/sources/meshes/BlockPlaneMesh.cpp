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
// Created by alexus on 13.03.25.
//

#include "KwasarEngine/meshes/BlockPlaneMesh.h"

#include "KwasarEngine/gltf/Accessor.h"
#include "KwasarEngine/gltf/Primitive.h"
#include "GLTFSDK/GLTF.h"

namespace mer::sdk {
BlockPlaneMesh::BlockPlaneMesh() {
	setName("BlockPlaneMesh");
	auto indexAccessor = Accessor::createIndexAccessor<uint16_t>({0, 1, 2, 1, 2, 3});
	auto posAccessor = Accessor::createVertexAccessor(
		{
			-0.5f, 0.0f, -0.5f, //0
			0.5f, 0.0f, -0.5f,  //1
			-0.5f, 0.0f, 0.5f,  //2
			0.5f, 0.0f, 0.5f
		}, //3
		Microsoft::glTF::TYPE_VEC3);

	const auto primitive = Primitive::create(MeshMode::MESH_TRIANGLES, posAccessor, indexAccessor);

	auto normalAccessor = Accessor::createVertexAccessor(
		{
			0.0f, 1.0f, 0.0f, //0
			0.0f, 1.0f, 0.0f, //1
			0.0f, 1.0f, 0.0f, //2
			0.0f, 1.0f, 0.0f
		}, //3
		Microsoft::glTF::TYPE_VEC3);

	primitive->setNormalAccessor(normalAccessor);
	addPrimitive(primitive);
}
} // namespace mer::sdk