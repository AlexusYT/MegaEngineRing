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
// Created by alexus on 24.06.24.
//

#ifndef ISCRIPT_H
#define ISCRIPT_H
#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk::main {
class IScript {
public:
	virtual ~IScript() = default;

	virtual utils::ReportMessagePtr setup() { return nullptr; }

	virtual void tick(double /*pDeltaTime*/) {}

	virtual void teardown() {}

	virtual void setObject(SceneObject* pSceneObject) = 0;
};
} // namespace mer::sdk::main


#endif //ISCRIPT_H