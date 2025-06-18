//  KwasarEngine is an SDK that can help you speed up game development.
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

#include "KwasarEngine/meshes/BlockSphereMesh.h"

#include "KwasarEngine/gltf/Accessor.h"
#include "KwasarEngine/gltf/Primitive.h"

namespace ke {
BlockSphereMesh::BlockSphereMesh() {
	setName("BlockPlaneMesh");
	auto [vertices, indices, normals] = generateSphere(1);
	auto indexAccessor = Accessor::createIndexAccessor<uint16_t>(indices);
	auto posAccessor = Accessor::createVertexAccessor(vertices, Microsoft::glTF::TYPE_VEC3);

	const auto primitive = Primitive::create(MeshMode::MESH_TRIANGLES, posAccessor, indexAccessor);

	auto normalAccessor = Accessor::createVertexAccessor(normals, Microsoft::glTF::TYPE_VEC3);

	primitive->setNormalAccessor(normalAccessor);
	addPrimitive(primitive);
}

std::tuple<std::vector<float>, std::vector<uint16_t>, std::vector<float>> BlockSphereMesh::generateSphere(
	const float pRadius,
	const int pNumSegments, const int pNumRings) {
	// Calculate vertices and normals
	std::vector<float> vertices;
	std::vector<float> normals;

	for (int i = 0; i < pNumRings; ++i) {
		// Polar angle
		float phi = std::numbers::pi_v<float> * static_cast<float>(i) / static_cast<float>(pNumRings);

		for (int j = 0; j < pNumSegments; ++j) {
			// Azimuthal angle
			float theta = 2.0f * std::numbers::pi_v<float> * static_cast<float>(j) / static_cast<float>(pNumSegments);


			// Calculate vertex position
			float x = pRadius * std::sin(phi) * std::cos(theta);
			float y = pRadius * std::cos(phi);
			float z = pRadius * std::sin(phi) * std::sin(theta);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Calculate normal (direction vector)
			float nx = std::sin(phi) * std::cos(theta);
			float ny = std::cos(phi);
			float nz = std::sin(phi) * std::sin(theta);

			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
		}
	}

	// Generate triangle indices
	std::vector<uint16_t> indices;

	for (int i = 0; i < pNumRings; ++i) {
		for (int j = 0; j < pNumSegments; ++j) {
			int idx1 = i * pNumSegments + j;
			int idx2 = (i + 1) * pNumSegments + j;
			int idx3 = (i + 1) * pNumSegments + ((j + 1) % pNumSegments);

			// First triangle
			indices.push_back(static_cast<uint16_t>(idx1));
			indices.push_back(static_cast<uint16_t>(idx2));
			indices.push_back(static_cast<uint16_t>(idx3));

			// Second triangle
			indices.push_back(static_cast<uint16_t>(idx1));
			indices.push_back(static_cast<uint16_t>(idx3));
			indices.push_back(static_cast<uint16_t>(i * pNumSegments + ((j + 1) % pNumSegments)));
		}
	}

	return {vertices, indices, normals};
}
}