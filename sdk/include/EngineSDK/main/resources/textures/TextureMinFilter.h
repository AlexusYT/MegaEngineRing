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

#ifndef TEXTUREMINFILTER_H
#define TEXTUREMINFILTER_H

namespace mer::sdk::main {

enum class TextureMinFilter {
	/**
	 * @brief Returns the value of the texture element that is nearest (in Manhattan distance) to the center of the
	 * pixel being textured.
	 */
	NEAREST = 0x2600,
	/**
	 * @brief Returns the weighted average of the four texture elements that are closest to the center of the pixel
	 * being textured.These can include border texture elements, depending on the values of TEXTURE_WRAP_S and
	 * TEXTURE_WRAP_T, and on the exact mapping
	 */
	LINEAR = 0x2601,

	/**
	 * @brief Chooses the mipmap that most closely matches the size of the pixel being textured and uses the NEAREST
	 * criterion (the texture element nearest to the center of the pixel) to produce a texture value.
	 */
	NEAREST_MIPMAP_NEAREST = 0x2700,
	/**
	 * @brief Chooses the mipmap that most closely matches the size of the pixel being textured and uses the LINEAR
	 * criterion (a weighted average of the four texture elements that are closest to the center of the pixel) to
	 * produce a texture value.
	 */
	LINEAR_MIPMAP_NEAREST = 0x2701,

	/**
	 * @brief Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the
	 * NEAREST criterion (the texture element nearest to the center of the pixel) to produce a texture value from
	 * each mipmap.The final texture value is a weighted average of those two values.
	 */
	NEAREST_MIPMAP_LINEAR = 0x2702,

	/**
	 * @brief Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the
	 * LINEAR criterion (a weighted average of the four texture elements that are closest to the center of the pixel)
	 * to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
	 */
	LINEAR_MIPMAP_LINEAR = 0x2703

};

} // namespace mer::sdk::main

#endif //TEXTUREMINFILTER_H
