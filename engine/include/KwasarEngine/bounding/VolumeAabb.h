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

#ifndef VOLUMEAABB_H
#define VOLUMEAABB_H
#include <glm/fwd.hpp>

#include "BoundingVolume.h"

namespace ke {
class Transformation;
}

namespace ke {
class VolumeAabb : public BoundingVolume {
	glm::vec3 min{};
	glm::vec3 max{};

	VolumeAabb(const glm::vec3 &pMin, const glm::vec3 &pMax) : min(pMin), max(pMax) {}

public:
	static std::shared_ptr<VolumeAabb> create(const glm::vec3 &pMin, const glm::vec3 &pMax);

	static std::shared_ptr<VolumeAabb> create() { return create({}, {}); }

	void setBounds(const glm::vec3 &pMin, const glm::vec3 &pMax) {
		if (pMin == min && pMax == max) return;
		min = pMin;
		max = pMax;
		updateVertices();
		notifyChanged();
	}

	void getBounds(glm::vec3 &pMinOut, glm::vec3 &pMaxOut) const {
		pMinOut = min;
		pMaxOut = max;
	}

	void setMin(const glm::vec3 &pMin) {
		if (pMin == min) return;
		min = pMin;
		updateVertices();
		notifyChanged();
	}

	void setMax(const glm::vec3 &pMax) {
		if (pMax == max) return;
		max = pMax;
		updateVertices();
		notifyChanged();
	}

	bool isIntersects(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, glm::vec3 &pIntersectCoordOut);

	void transform(const glm::mat4 &pTransformMatrix, glm::vec3 &pNewMin, glm::vec3 &pNewMax) const;

	[[nodiscard]] const glm::vec3 &getMin() const { return min; }

	[[nodiscard]] const glm::vec3 &getMax() const { return max; }

protected:
	std::vector<float> getVertices() override;

	std::vector<uint16_t> getIndices() override;
};
} // namespace ke

#endif //VOLUMEAABB_H
