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

#ifndef BOUNDINGVOLUMEPROGRAM_H
#define BOUNDINGVOLUMEPROGRAM_H
#include "EngineSDK/resources/shaders/ShaderProgram.h"

namespace mer::sdk {

class BoundingVolumeProgram : public ShaderProgram {

	static std::shared_ptr<BoundingVolumeProgram> instance;
	BoundingVolumeProgram();

public:
	static std::shared_ptr<BoundingVolumeProgram> getInstance();
};

} // namespace mer::sdk

#endif //BOUNDINGVOLUMEPROGRAM_H
