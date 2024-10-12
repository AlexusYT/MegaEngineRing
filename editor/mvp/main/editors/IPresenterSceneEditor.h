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
// Created by alexus on 20.01.24.
//

#ifndef IEDITOR_H
#define IEDITOR_H
#include "mvp/PresenterBase.h"

namespace mer::sdk::main {
class ISceneObject;
}

namespace mer::editor::mvp {

class IPresenterSceneEditor : public PresenterBase {
public:
	~IPresenterSceneEditor() override = default;

	virtual bool onMouseScroll(double pDx, double pDy) = 0;
};

} // namespace mer::editor::mvp

#endif //IEDITOR_H
