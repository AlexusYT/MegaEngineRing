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

#ifndef TEXTUREBASEINTERNALFORMAT_H
#define TEXTUREBASEINTERNALFORMAT_H

namespace mer::sdk {
enum class TextureBaseInternalFormat {
	DEPTH_COMPONENT = 0x1902,
	DEPTH_STENCIL   = 0x84F9,
	RED             = 0x1903,
	RG              = 0x8227,
	RGB             = 0x1907,
	RGBA            = 0x1908
};
} // namespace mer::sdk

#endif //TEXTUREBASEINTERNALFORMAT_H
