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
// Created by alexus on 13.06.2025.
//

#include "EngineSDK/meshes/BlockCubeMesh.h"

#include "EngineSDK/gltf/Accessor.h"
#include "EngineSDK/gltf/Primitive.h"

namespace mer::sdk {
BlockCubeMesh::BlockCubeMesh() {
	setName("BlockCubeMesh");
	auto indexAccessor = Accessor::createIndexAccessor<uint16_t>({
		4, 2, 0,
		2, 7, 3,
		6, 5, 7,
		1, 7, 5,
		0, 3, 1,

		4, 1, 5,
		4, 6, 2,
		2, 6, 7,
		6, 4, 5,
		1, 3, 7,
		0, 2, 3,
		4, 0, 1,
	});
	auto posAccessor = Accessor::createVertexAccessor(
		{
			0.5f, 0.5f, -0.5f,   // 0
			0.5f, -0.5f, -0.5f,  // 1
			0.5f, 0.5f, 0.5f,    // 2
			0.5f, -0.5f, 0.5f,   // 3
			-0.5f, 0.5f, -0.5f,  // 4
			-0.5f, -0.5f, -0.5f, // 5
			-0.5f, 0.5f, 0.5f,   // 6
			-0.5f, -0.5f, 0.5f   // 7
		}
		,
		Microsoft::glTF::TYPE_VEC3);

	const auto primitive = Primitive::create(MeshMode::MESH_TRIANGLES, posAccessor, indexAccessor);

	auto normalAccessor = Accessor::createVertexAccessor(
		{
			0.0f, 0.0f, 1.0f, // 0
			0.0f, 0.0f, 1.0f, // 1
			0.0f, 0.0f, 1.0f, // 2
			0.0f, 0.0f, 1.0f, // 3

			0.0f, 0.0f, -1.0f, // 4
			0.0f, 0.0f, -1.0f, // 5
			0.0f, 0.0f, -1.0f, // 6
			0.0f, 0.0f, -1.0f, // 7

			-1.0f, 0.0f, 0.0f, // 0
			-1.0f, 0.0f, 0.0f, // 4
			-1.0f, 0.0f, 0.0f, // 6
			-1.0f, 0.0f, 0.0f, // 2

			1.0f, 0.0f, 0.0f, // 1
			1.0f, 0.0f, 0.0f, // 5
			1.0f, 0.0f, 0.0f, // 7
			1.0f, 0.0f, 0.0f, // 3

			0.0f, 1.0f, 0.0f, // 2
			0.0f, 1.0f, 0.0f, // 3
			0.0f, 1.0f, 0.0f, // 7
			0.0f, 1.0f, 0.0f, // 6

			0.0f, -1.0f, 0.0f, // 0
			0.0f, -1.0f, 0.0f, // 1
			0.0f, -1.0f, 0.0f, // 4
			0.0f, -1.0f, 0.0f, // 5
		},
		Microsoft::glTF::TYPE_VEC3);

	primitive->setNormalAccessor(normalAccessor);
	addPrimitive(primitive);
}
}
