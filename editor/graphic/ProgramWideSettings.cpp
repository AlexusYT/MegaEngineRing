// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 12.07.23.
//
#if 0

	#include "ProgramWideSettings.h"

	#include <Render/opengl/buffers/SSBO.h>

ProgramWideSettings::ProgramWideSettings() {}

bool ProgramWideSettings::init() {
	ssbo = rel::opengl::SSBO::Builder().setBufferData(sizeof(data), &data).setUsage(rel::opengl::DYNAMIC_DRAW).build();
	if (!ssbo) {
		std::cerr << "Not enough memory to allocate a new instance of the SSBO" << std::endl;
		return false;
	}
	return true;
}

void ProgramWideSettings::update() {
	if (raw) {
		raw = false;
		ssbo->bufferSubDataBind(0, sizeof(data), &data);
	}
}

#endif
