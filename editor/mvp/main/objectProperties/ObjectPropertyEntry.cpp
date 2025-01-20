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
// Created by alexus on 17.03.24.
//

#include "ObjectPropertyEntry.h"

#include "EngineSDK/extensions/Extension.h"
#include "PropertyRenderer.h"

namespace mer::editor::mvp {
const std::string &ObjectPropertyEntry::getName() { return nativeProperty->getPropertyName(); }

std::shared_ptr<PropertyRenderer> ObjectPropertyEntry::getRenderer() const {
	auto resourceLoader = extension->getScene()->getResourceExecutor();
	return PropertyRenderer::create(nativeProperty, resourceLoader);
}

ObjectExtensionEntry::ObjectExtensionEntry(sdk::main::Extension* pNativeExtension)
	: ObjectPropertyEntryBase(Gio::ListStore<ObjectPropertyEntry>::create()), nativeExtension(pNativeExtension) {
	const auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ObjectPropertyEntry>>(getChildren());
	for (const auto property: pNativeExtension->getProperties()) {
		childrenSelf->append(ObjectPropertyEntry::create(property, pNativeExtension));
	}
}

const std::string &ObjectExtensionEntry::getName() { return nativeExtension->getName(); }
} // namespace mer::editor::mvp