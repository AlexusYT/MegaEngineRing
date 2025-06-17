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
// Created by alexus on 25.02.24.
//

#ifndef KEYBOARDKEY_H
#define KEYBOARDKEY_H
#include "PropertyJsonSerializer.h"

namespace ke {
enum class KeyboardKey {
	KEY_UNKNOWN       = -1,
	KEY_ESCAPE        = 9,
	KEY_1             = 10,
	KEY_2             = 11,
	KEY_3             = 12,
	KEY_4             = 13,
	KEY_5             = 14,
	KEY_6             = 15,
	KEY_7             = 16,
	KEY_8             = 17,
	KEY_9             = 18,
	KEY_0             = 19,
	KEY_MINUS         = 20, /* - */
	KEY_EQUAL         = 21, /* = */
	KEY_BACKSPACE     = 22,
	KEY_TAB           = 23,
	KEY_Q             = 24,
	KEY_W             = 25,
	KEY_E             = 26,
	KEY_R             = 27,
	KEY_T             = 28,
	KEY_Y             = 29,
	KEY_U             = 30,
	KEY_I             = 31,
	KEY_O             = 32,
	KEY_P             = 33,
	KEY_LEFT_BRACKET  = 34, /* [ */
	KEY_RIGHT_BRACKET = 35, /* ] */
	KEY_LEFT_CONTROL  = 37,
	KEY_A             = 38,
	KEY_S             = 39,
	KEY_D             = 40,
	KEY_F             = 41,
	KEY_G             = 42,
	KEY_H             = 43,
	KEY_J             = 44,
	KEY_K             = 45,
	KEY_L             = 46,
	KEY_SEMICOLON     = 47, /* ; */
	KEY_APOSTROPHE    = 48, /* ' */
	KEY_GRAVE_ACCENT  = 49, /* ` */
	KEY_LEFT_SHIFT    = 50,
	KEY_BACKSLASH     = 51, /* \ */
	KEY_Z             = 52,
	KEY_X             = 53,
	KEY_C             = 54,
	KEY_V             = 55,
	KEY_B             = 56,
	KEY_N             = 57,
	KEY_M             = 58,
	KEY_COMMA         = 59, /* , */
	KEY_PERIOD        = 60, /* . */
	KEY_SLASH         = 61, /* / */
	KEY_RIGHT_SHIFT   = 62,
	KEY_LEFT_ALT      = 64,
	KEY_F1            = 67,
	KEY_F2            = 68,
	KEY_F3            = 69,
	KEY_F4            = 70,
	KEY_F5            = 71,
	KEY_F6            = 72,
	KEY_F7            = 73,
	KEY_F8            = 74,
	KEY_F9            = 75,
	KEY_F10           = 76,
	KEY_F11           = 95,
	KEY_F12           = 96,
};

template <>
struct PropertyJsonSerializer<KeyboardKey> {
	static nlohmann::json serialize(const KeyboardKey &pValue, Extension* /*pExtension*/) {
		return static_cast<int>(pValue);
	}

	static KeyboardKey deserialize(const nlohmann::json &pValue, Extension* /*pExtension*/) {
		return static_cast<KeyboardKey>(pValue.get<int>());
	}
};
} // namespace ke

#endif //KEYBOARDKEY_H
