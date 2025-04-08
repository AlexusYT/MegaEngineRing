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
// Created by alexus on 25.02.25.
//

#include "EngineSDK/gltf/Mesh.h"

#include "EngineSDK/gltf/Primitive.h"
#include "EngineSDK/bounding/VolumeAabb.h"

namespace mer::sdk {
Mesh::Mesh() : aabb(nullptr, "AABB") {
	aabb = VolumeAabb::create();
	buildAabb();
}

std::shared_ptr<Mesh> Mesh::create() { return std::shared_ptr<Mesh>(new Mesh()); }

void Mesh::addPrimitive(const std::shared_ptr<Primitive> &pPrimitive) {
	primitives.emplace_back(pPrimitive);

	connections.emplace_back(
		pPrimitive->getAabb().connectEvent([this](const std::shared_ptr<VolumeAabb> & /*pValue*/) { buildAabb(); }));
	buildAabb();
}

void Mesh::setPrimitives(const std::vector<std::shared_ptr<Primitive>> &pPrimitives) {
	primitives = pPrimitives;
	connections.clear();
	for (auto primitive: primitives) {
		connections.emplace_back(
			primitive->getAabb().connectEvent([this](const std::shared_ptr<VolumeAabb> & /*pValue*/) { buildAabb(); }));
	}
	buildAabb();
}

void Mesh::buildAabb() const {

	glm::vec3 min(std::numeric_limits<float>::max());
	glm::vec3 max(std::numeric_limits<float>::lowest());
	bool minDefined{};
	bool maxDefined{};
	for (const auto &primitive: primitives) {
		auto primAabb = primitive->getAabb().getValue();

		glm::vec3 primMin, primMax;
		primAabb->getBounds(primMin, primMax);
		if (min > primMin) {
			min = primMin;
			minDefined = true;
		}
		if (max < primMax) {
			max = primMax;
			maxDefined = true;
		}
	}

	if (minDefined && maxDefined) aabb->setBounds(min, max);
}
} // namespace mer::sdk