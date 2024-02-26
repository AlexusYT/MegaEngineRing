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
// Created by alexus on 24.02.24.
//

#ifndef IACTOR_H
#define IACTOR_H
#include "EngineSDK/utils/KeyboardKey.h"
#include "EngineSDK/utils/ModifierKeys.h"

namespace mer::sdk::main {

class IActor {
public:
	virtual ~IActor() = default;

	virtual bool onCursorPosChanged(double pX, double pY);

	virtual void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, utils::ModifierKeys pMods);

	virtual void update();
};

inline bool IActor::onCursorPosChanged(const double /*pX*/, const double /*pY*/) { return true; }

inline void IActor::onKeyStateChanged(utils::KeyboardKey /*pKey*/, bool /*pPressed*/, utils::ModifierKeys /*pMods*/) {}

inline void IActor::update() {}

} // namespace mer::sdk::main

#endif //IACTOR_H
