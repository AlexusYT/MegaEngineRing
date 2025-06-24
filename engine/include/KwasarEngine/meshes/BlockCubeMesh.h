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

#ifndef BLOCKCUBEMESH_H
#define BLOCKCUBEMESH_H
#include "KwasarEngine/gltf/Mesh.h"

namespace ke {
class BlockCubeMesh : public Mesh {
protected:
	BlockCubeMesh();

public:
	static std::shared_ptr<BlockCubeMesh> create() { return std::shared_ptr<BlockCubeMesh>(new BlockCubeMesh()); }
};
}

#endif //BLOCKCUBEMESH_H
