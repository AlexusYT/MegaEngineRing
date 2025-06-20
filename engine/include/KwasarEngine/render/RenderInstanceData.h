//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 02.10.24.
//

#ifndef RENDERINSTANCEDATA_H
#define RENDERINSTANCEDATA_H
#include <glm/mat4x4.hpp>

#include "KwasarEngine/resources/materials/MaterialData.h"

namespace ke {
class RenderInstanceData {
public:
	MaterialData material;
	glm::mat4 modelViewMatrix{1};
	glm::mat4 normalMatrix{1};
};
} // namespace ke

#endif //RENDERINSTANCEDATA_H