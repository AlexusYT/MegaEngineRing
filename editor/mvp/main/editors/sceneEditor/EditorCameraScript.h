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
// Created by alexus on 11.10.24.
//

#ifndef EDITORCAMERASCRIPT_H
#define EDITORCAMERASCRIPT_H
#include "EngineSDK/scripting/Script.h"

namespace mer::sdk {
class OrbitCameraExtension;
}

namespace mer::editor::mvp {

class EditorCameraScript : public sdk::Script {
	std::shared_ptr<sdk::OrbitCameraExtension> camera;

public:
	sdk::ReportMessagePtr setup() override;

protected:
	inline bool onMouseScroll(double pDx, double pDy) override;
};

} // namespace mer::editor::mvp

#endif //EDITORCAMERASCRIPT_H
