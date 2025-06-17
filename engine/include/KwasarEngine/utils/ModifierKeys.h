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
// Created by alexus on 25.02.24.
//

#ifndef MODIFIERKEYS_H
#define MODIFIERKEYS_H
#include <cstdint>

namespace ke {
class ModifierKeys {
	uint8_t mods;

public:
	explicit ModifierKeys(const uint8_t pMods) : mods(pMods) {}

	///If this bit is set one or more Shift keys were held down.
	static constexpr uint8_t MOD_SHIFT = 0b0001;

	///If this bit is set one or more Control keys were held down.
	static constexpr uint8_t MOD_CONTROL = 0b0010;

	///If this bit is set one or more Alt keys were held down.
	static constexpr uint8_t MOD_ALT = 0b0100;

	///If this bit is set one or more Super keys were held down.
	static constexpr uint8_t MOD_SUPER = 0b1000;

	///If this bit is set the Caps Lock key is enabled.
	static constexpr uint8_t MOD_CAPS_LOCK = 0b10000;

	///If this bit is set the Num Lock key is enabled.
	static constexpr uint8_t MOD_NUM_LOCK = 0b100000;

	/// @return bit field of mods that applied to the key
	[[nodiscard]] uint8_t getMods() const { return mods; }

	void setMods(const uint8_t pMods) { mods = pMods; }

	/// @return true if one or more Shift keys were held down
	[[nodiscard]] bool isShift() const { return mods & MOD_SHIFT; }

	/// @return true if one or more Control keys were held down
	[[nodiscard]] bool isControl() const { return mods & MOD_CONTROL; }

	/// @return true if one or more Alt keys were held down
	[[nodiscard]] bool isAlt() const { return mods & MOD_ALT; }

	/// @return true if one or more Super keys were held down
	[[nodiscard]] bool isSuper() const { return mods & MOD_SUPER; }

	/// @return true if Caps Lock key is enabled
	[[nodiscard]] bool isCapsLock() const { return mods & MOD_CAPS_LOCK; }

	/// @return true if Num Lock key is enabled
	[[nodiscard]] bool isNumLock() const { return mods & MOD_NUM_LOCK; }

	/**
	 * @brief Checks if specified mods applied to the key.
	 * @param modsToCheck bit field of mods to check.
	 * @return true if specified mods applied to the key, false otherwise.
	 */
	[[nodiscard]] bool check(const uint8_t modsToCheck) const { return mods & modsToCheck; }
};
} // namespace ke


#endif //MODIFIERKEYS_H
