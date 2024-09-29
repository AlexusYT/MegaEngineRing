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
// Created by alexus on 17.03.24.
//

#ifndef OBJECTPROPERTYENTRY_H
#define OBJECTPROPERTYENTRY_H
#include "EngineSDK/main/scene/objects/extensions/Extension.h"
#include "ui/customWidgets/TreeElementBase.h"


class ObjectPropertyEntry;

namespace mer::sdk::main {
class Extension;
class ExtensionPropertyBase;
} // namespace mer::sdk::main

namespace mer::editor::mvp {
class ObjectPropertyEntryBase : public ui::TreeElementBase {
public:
	explicit ObjectPropertyEntryBase(const std::shared_ptr<Gio::ListModel> &pChildren) : TreeElementBase(pChildren) {}

	virtual const std::string &getName() = 0;
};

class ObjectPropertyEntry : public ObjectPropertyEntryBase {
	sdk::main::Extension* extension;
	sdk::main::ExtensionPropertyBase* nativeProperty;

protected:
	explicit ObjectPropertyEntry(sdk::main::ExtensionPropertyBase* pNativeProperty, sdk::main::Extension* pExtension)
		: ObjectPropertyEntryBase(nullptr), extension(pExtension), nativeProperty(pNativeProperty) {}

public:
	static std::shared_ptr<ObjectPropertyEntry> create(sdk::main::ExtensionPropertyBase* pProp,
													   sdk::main::Extension* pExtension) {
		return Glib::make_refptr_for_instance(new ObjectPropertyEntry(pProp, pExtension));
	}

	std::shared_ptr<Gio::MenuModel> getMenu() override { return nullptr; }

	[[nodiscard]] sdk::main::ExtensionPropertyBase* getNativeProperty() const { return nativeProperty; }

	const std::string &getName() override { return nativeProperty->getPropertyName(); }

	[[nodiscard]] sdk::main::Extension* getExtension() const { return extension; }
};

class ObjectExtensionEntry : public ObjectPropertyEntryBase {
	sdk::main::Extension* nativeExtension{};

protected:
	explicit ObjectExtensionEntry(sdk::main::Extension* pNativeExtension)
		: ObjectPropertyEntryBase(Gio::ListStore<ObjectPropertyEntry>::create()), nativeExtension(pNativeExtension) {
		const auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ObjectPropertyEntry>>(getChildren());
		for (const auto property: pNativeExtension->getProperties()) {
			childrenSelf->append(ObjectPropertyEntry::create(property, pNativeExtension));
		}
	}

public:
	static std::shared_ptr<ObjectExtensionEntry> create(sdk::main::Extension* pNativeExtension) {
		return make_refptr_for_instance(new ObjectExtensionEntry(pNativeExtension));
	}

	std::shared_ptr<Gio::MenuModel> getMenu() override { return nullptr; }

	const std::string &getName() override { return nativeExtension->getName(); }

	[[nodiscard]] sdk::main::Extension* getNativeExtension() const { return nativeExtension; }
};

} // namespace mer::editor::mvp

#endif //OBJECTPROPERTYENTRY_H
