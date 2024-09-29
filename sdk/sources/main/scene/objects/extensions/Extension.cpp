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
// Created by alexus on 27.02.24.
//


#include <EngineSDK/main/scene/objects/extensions/Extension.h>

#include <nlohmann/json.hpp>

#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"
#include "EngineUtils/utils/Logger.h"

namespace mer::sdk::main {

Extension::Extension() {}

IScene* Extension::getScene() const { return getObject()->getScene(); }

void Extension::onWindowSizeChanged(int /*pWidth*/, int /*pHeight*/) {}

void Extension::onCursorPosChanged(const double /*pX*/, const double /*pY*/) {}

void Extension::onKeyStateChanged(const utils::KeyboardKey /*pKey*/, const bool /*pPressed*/,
								  const utils::ModifierKeys & /*pMods*/) {}

void Extension::onMouseButtonStateChanged(utils::MouseButton /*pButton*/, bool /*pPressed*/, double /*pX*/,
										  double /*pY*/) const {}

void Extension::onEnabledChanged() {}

void Extension::getTypeNameFor(Extension* pExt, std::string &pNameOut) {
	if (auto msg = ExtensionRegistry::getTypeNameFor(pExt, pNameOut)) { utils::Logger::error(msg); }
}

void Extension::addProperty(ExtensionPropertyBase* pProperty) { properties.push_back(pProperty); }

void Extension::removeProperty(ExtensionPropertyBase* pProperty) { erase(properties, pProperty); }

void Extension::propertyChanged(ExtensionPropertyBase* pProperty) {
	if (object) object->notifyExtensionPropertyChanged(this, pProperty);
}

} // namespace mer::sdk::main