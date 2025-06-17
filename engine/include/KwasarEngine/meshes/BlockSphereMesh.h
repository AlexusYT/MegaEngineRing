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

#ifndef BLOCKSPHEREMESH_H
#define BLOCKSPHEREMESH_H
#include "KwasarEngine/gltf/Mesh.h"

namespace ke {
class BlockSphereMesh : public Mesh {
public:
	BlockSphereMesh();

protected:
	static std::tuple<std::vector<float>, std::vector<uint16_t>, std::vector<float>> generateSphere(
		float pRadius, int pNumSegments = 32, int pNumRings = 16);
};
}

#endif //BLOCKSPHEREMESH_H