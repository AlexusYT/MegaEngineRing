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
// Created by alexus on 12.10.24.
//

#ifndef TEXTURE2DTYPE_H
#define TEXTURE2DTYPE_H

namespace mer::sdk {

enum class Texture2DType {

	UNSIGNED_BYTE = 0x1401,
	BYTE = 0x1400,
	UNSIGNED_SHORT = 0x1403,
	SHORT = 0x1402,
	UNSIGNED_INT = 0x1405,
	INT = 0x1404,
	FLOAT = 0x1406,
	UNSIGNED_BYTE_3_3_2 = 0x8032,
	UNSIGNED_BYTE_2_3_3_REV = 0x8362,
	UNSIGNED_SHORT_5_6_5 = 0x8363,
	UNSIGNED_SHORT_5_6_5_REV = 0x8364,
	UNSIGNED_SHORT_4_4_4_4 = 0x8033,
	UNSIGNED_SHORT_4_4_4_4_REV = 0x8365,
	UNSIGNED_SHORT_5_5_5_1 = 0x8034,
	UNSIGNED_SHORT_1_5_5_5_REV = 0x8366,
	UNSIGNED_INT_8_8_8_8 = 0x8035,
	UNSIGNED_INT_8_8_8_8_REV = 0x8367,
	UNSIGNED_INT_10_10_10_2 = 0x8036,
	UNSIGNED_INT_2_10_10_10_REV = 0x8368

};

}

#endif //TEXTURE2DTYPE_H
