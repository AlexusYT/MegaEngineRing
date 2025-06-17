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
// Created by alexus on 11.03.25.
//

#ifndef DEBUGGEOMETRY_H
#define DEBUGGEOMETRY_H
#include <vector>

#include "BoundingVolume.h"

class DebugGeometry : public ke::BoundingVolume {
	std::vector<uint16_t> indices;
	std::vector<float> vertices;

public:
	void setIndices(const std::vector<uint16_t> &pIndices) {
		indices = pIndices;
		updateIndices();
	}

	void setVertices(const std::vector<float> &pVertices) {
		vertices = pVertices;
		updateVertices();
	}

	void setCube(const glm::vec3 &pCubePosition) {
		glm::vec3 min = pCubePosition + glm::vec3(-0.1f);
		glm::vec3 max = pCubePosition + glm::vec3(0.1f);

		setVertices({
			//
			min.x, min.y, min.z, //0
			max.x, min.y, min.z, //1
			max.x, max.y, min.z, //2
			min.x, max.y, min.z, //3

			min.x, min.y, max.z, //4
			max.x, min.y, max.z, //5
			min.x, max.y, max.z, //6
			max.x, max.y, max.z, //7
		});
		setIndices({0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 5, 5, 1, 5, 7, 7, 2, 3, 6, 4, 6, 7, 6});
	}

	std::vector<float> getVertices() override { return vertices; }

	std::vector<uint16_t> getIndices() override { return indices; }
};


#endif //DEBUGGEOMETRY_H
