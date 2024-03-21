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
// Created by alexus on 12.02.24.
//

#ifndef ISCENEOBJECT_H
#define ISCENEOBJECT_H

#include <EngineUtils/utils/ReportMessageFwd.h>
#include <map>


class UUID;

namespace mer::sdk::utils {
class ModifierKeys;
enum class KeyboardKey;
} // namespace mer::sdk::utils

namespace mer::sdk::main {
class Extension;
class IScene;

class ISceneObject {
public:
	virtual ~ISceneObject() = default;

	virtual utils::ReportMessagePtr init() = 0;

	virtual void render() const = 0;

	virtual void onWindowSizeChanged(int pWidth, int pHeight) const = 0;

	virtual void onCursorPosChanged(double pX, double pY) const = 0;

	virtual void onKeyStateChanged(utils::KeyboardKey pKey, bool pPressed, const utils::ModifierKeys &pMods) const = 0;

	virtual void setScene(IScene* pScene) = 0;

	virtual UUID* getUuid() = 0;

	virtual const std::map<std::string, std::shared_ptr<Extension>> &getExtensions() const = 0;

	[[nodiscard]] virtual const glm::vec3 &getPosition() const = 0;

	virtual void setPosition(const glm::vec3 &pPosition) = 0;

	[[nodiscard]] virtual const std::string &getName() const = 0;

	virtual void setName(const std::string &pName) = 0;
};

} // namespace mer::sdk::main

#endif //ISCENEOBJECT_H
