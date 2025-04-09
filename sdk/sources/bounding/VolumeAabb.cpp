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

#include "EngineSDK/bounding/VolumeAabb.h"

#include <cmath>

namespace mer::sdk {
std::shared_ptr<VolumeAabb> VolumeAabb::create(const glm::vec3 &pMin, const glm::vec3 &pMax) {
	return std::shared_ptr<VolumeAabb>(new VolumeAabb(pMin, pMax));
}

enum { NUMDIM = 3, RIGHT = 0, LEFT = 1, MIDDLE = 2 };

bool VolumeAabb::isIntersects(const glm::vec3 &pRayOrigin, const glm::vec3 &pRayDir, glm::vec3 &pIntersectCoordOut) {
	auto minB = min;
	auto maxB = max;
	char inside = true;
	char quadrant[NUMDIM];
	int i;
	int whichPlane;
	float maxT[NUMDIM];
	float candidatePlane[NUMDIM]{};

	/* Find candidate planes; this loop can be avoided if
	   rays cast all from the eye(assume perpsective view) */
	for (i = 0; i < NUMDIM; i++)
		if (pRayOrigin[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		} else if (pRayOrigin[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		} else {
			quadrant[i] = MIDDLE;
		}

	/* Ray origin inside bounding box */
	if (inside) {
		pIntersectCoordOut = pRayOrigin;
		return (true);
	}


	/* Calculate T distances to candidate planes */
	for (i = 0; i < NUMDIM; i++)
		if (quadrant[i] != MIDDLE && pRayDir[i] != 0.f) maxT[i] = (candidatePlane[i] - pRayOrigin[i]) / pRayDir[i];
		else
			maxT[i] = -1.f;

	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < NUMDIM; i++)
		if (maxT[whichPlane] < maxT[i]) whichPlane = i;

	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.f) return (false);
	for (i = 0; i < NUMDIM; i++)
		if (whichPlane != i) {
			pIntersectCoordOut[i] = pRayOrigin[i] + maxT[whichPlane] * pRayDir[i];
			if (pIntersectCoordOut[i] < minB[i] || pIntersectCoordOut[i] > maxB[i]) return (false);
		} else {
			pIntersectCoordOut[i] = candidatePlane[i];
		}
	return (true); /* ray hits box */
				   /*float t0 = 0;
	float t1 = 1000;
	Ray r(Vector3(pRayOrigin.x, pRayOrigin.y, pRayOrigin.z), Vector3(pRayDir.x, pRayDir.y, pRayDir.z));
	Vector3 parameters[2];
	parameters[0] = Vector3(min.x, min.y, min.z);
	parameters[1] = Vector3(max.x, max.y, max.z);

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (parameters[r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
	tmax = (parameters[1 - r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
	tymin = (parameters[r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
	tymax = (parameters[1 - r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;
	tzmin = (parameters[r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
	tzmax = (parameters[1 - r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	return ((tmin < t1) && (tmax > t0));*/
}

std::vector<float> VolumeAabb::getVertices() {
	return {
		//
		min.x, min.y, min.z, //0
		max.x, min.y, min.z, //1
		max.x, max.y, min.z, //2
		min.x, max.y, min.z, //3

		min.x, min.y, max.z, //4
		max.x, min.y, max.z, //5
		min.x, max.y, max.z, //6
		max.x, max.y, max.z, //7
	};
}

std::vector<uint16_t> VolumeAabb::getIndices() {
	return {0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 5, 5, 1, 5, 7, 7, 2, 3, 6, 4, 6, 7, 6};
}

} // namespace mer::sdk