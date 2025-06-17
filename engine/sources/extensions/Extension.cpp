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
// Created by alexus on 27.02.24.
//


#include <KwasarEngine/extensions/Extension.h>

#include <nlohmann/json.hpp>

#include "KwasarEngine/extensions/ExtensionRegistry.h"
#include "KwasarEngine/scene/objects/SceneObject.h"
#include "KwasarEngine/utils/Logger.h"
#include "KwasarEngine/utils/PropertyBase.h"

namespace ke {
Extension::Extension() {}

void Extension::serialize(nlohmann::json &pJson) {
	for (const auto property: properties) {
		if (const auto serializable = dynamic_cast<ISerializable*>(property)) serializable->serialize(pJson, this);
	}
}

void Extension::deserialize(const nlohmann::json &pJson) {
	for (const auto property: properties) {
		if (const auto serializable = dynamic_cast<ISerializable*>(property)) serializable->deserialize(pJson, this);
	}
}

IScene* Extension::getScene() const { return getObject()->getScene(); }

bool Extension::notifyOnMouseScroll(double pDx, double pDy) { return onMouseScroll(pDx, pDy); }

void Extension::onNodeChanged(Node* /*pOldNode*/) {}

void Extension::onWindowSizeChanged(int /*pWidth*/, int /*pHeight*/) {}

void Extension::onCursorPosChanged(const double /*pX*/, const double /*pY*/) {}

void Extension::onKeyStateChanged(const KeyboardKey /*pKey*/, const bool /*pPressed*/,
								  const ModifierKeys & /*pMods*/) {}

void Extension::onMouseButtonStateChanged(MouseButton /*pButton*/, bool /*pPressed*/, double /*pX*/,
										  double /*pY*/) const {}

void Extension::onEnabledChanged() {}

bool Extension::onMouseScroll(double /*pDx*/, double /*pDy*/) { return false; }

void Extension::getTypeNameFor(Extension* pExt, std::string &pNameOut) {
	if (auto msg = ExtensionRegistry::getTypeNameFor(pExt, pNameOut)) { Logger::error(msg); }
}

void Extension::addProperty(PropertyBase* pProperty) { properties.push_back(pProperty); }

void Extension::removeProperty(PropertyBase* pProperty) { erase(properties, pProperty); }

void Extension::propertyChanged(PropertyBase* pProperty) {
	if (object) object->notifyExtensionPropertyChanged(this, pProperty);
}
} // namespace ke