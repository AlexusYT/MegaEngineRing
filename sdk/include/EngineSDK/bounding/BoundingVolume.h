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
// Created by alexus on 06.03.25.
//

#ifndef IBOUNDINGVOLUME_H
#define IBOUNDINGVOLUME_H
#include "EngineSDK/render/IRenderable.h"
#include "EngineSDK/render/Initializable.h"
#include "EngineUtils/utils/PropertyNotifiable.h"

namespace mer::sdk {
class BoundingVolume : public IRenderable, public Initializable, public PropertyNotifiable {
	uint32_t vao{};
	uint32_t verticesBuffer{};
	bool shouldUpdateVertices{true};
	uint32_t indicesBuffer{};
	bool shouldUpdateIndices{true};
	size_t indicesCount{};

public:
	virtual ~BoundingVolume() = default;
	void render() override;

protected:
	void updateVertices();

	void updateIndices();

	virtual std::vector<float> getVertices() = 0;

	virtual std::vector<uint16_t> getIndices() = 0;

	ReportMessagePtr onInitialize() override;

	void onUninitialize() override;
};
} // namespace mer::sdk

#endif //IBOUNDINGVOLUME_H
