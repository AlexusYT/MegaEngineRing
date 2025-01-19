//  MegaEngineRing is a program that can speed up game development.
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
// Created by alexus on 04.02.24.
//

#ifndef BUFFERUSAGEENUM_H
#define BUFFERUSAGEENUM_H

namespace mer::sdk::main {

enum BufferUsageEnum : uint32_t {
	STREAM_DRAW = 0x88E0,
	STREAM_READ = 0x88E1,
	STREAM_COPY = 0x88E2,
	STATIC_DRAW = 0x88E4,
	STATIC_READ = 0x88E5,
	STATIC_COPY = 0x88E6,
	DYNAMIC_DRAW = 0x88E8,
	DYNAMIC_READ = 0x88E9,
	DYNAMIC_COPY = 0x88EA
};
}


#endif //BUFFERUSAGEENUM_H
