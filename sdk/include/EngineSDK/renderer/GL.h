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
// Created by alexus on 13.02.24.
//

#ifndef GL_H
#define GL_H
#include <cstdint>

namespace mer::sdk::main {

enum class ClearBits : uint32_t {

	COLOR_BUFFER_BIT,
	DEPTH_BUFFER_BIT,
	STENCIL_BUFFER_BIT


};

constexpr ClearBits operator&(ClearBits pX, ClearBits pY) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(static_cast<utype>(pX) & static_cast<utype>(pY));
}

constexpr ClearBits operator|(ClearBits pX, ClearBits pY) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(static_cast<utype>(pX) | static_cast<utype>(pY));
}

constexpr ClearBits operator^(ClearBits pX, ClearBits pY) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(static_cast<utype>(pX) ^ static_cast<utype>(pY));
}

constexpr ClearBits operator~(ClearBits pX) noexcept {
	using utype = std::underlying_type_t<ClearBits>;
	return static_cast<ClearBits>(~static_cast<utype>(pX));
}

inline ClearBits &operator&=(ClearBits &pX, ClearBits pY) noexcept { return pX = pX & pY; }

inline ClearBits &operator|=(ClearBits &pX, ClearBits pY) noexcept { return pX = pX | pY; }

inline ClearBits &operator^=(ClearBits &pX, ClearBits pY) noexcept { return pX = pX ^ pY; }

class GL {
public:
	static void viewport(int pX, int pY, int pWidth, int pHeight);

	static void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha);

	static void clear(ClearBits pMask);
};

} // namespace mer::sdk::main


#endif //GL_H
