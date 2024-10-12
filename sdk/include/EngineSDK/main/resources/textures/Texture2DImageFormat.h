//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

#ifndef TEXTURE2DIMAGEFORMAT_H
#define TEXTURE2DIMAGEFORMAT_H

namespace mer::sdk::main {

enum class Texture2DImageFormat {

	RED = 0x1903,
	RG = 0x8227,
	RGB = 0x1907,
	BGR = 0x80E0,
	RGBA = 0x1908,
	BGRA = 0x80E1,
	RED_INTEGER = 0x8D94,
	RG_INTEGER = 0x8228,
	RGB_INTEGER = 0x8D98,
	BGR_INTEGER = 0x8D9A,
	RGBA_INTEGER = 0x8D99,
	BGRA_INTEGER = 0x8D9B,
	STENCIL_INDEX = 0x1901,
	DEPTH_COMPONENT = 0x1902,
	DEPTH_STENCIL = 0x84F9

};

}

#endif //TEXTURE2DIMAGEFORMAT_H
