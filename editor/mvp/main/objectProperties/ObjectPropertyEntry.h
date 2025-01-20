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

#ifndef OBJECTPROPERTYENTRY_H
#define OBJECTPROPERTYENTRY_H
#include "ui/customWidgets/TreeElementBase.h"

namespace mer::sdk {
class PropertyBase;
}

namespace mer::sdk {
class Extension;
} // namespace mer::sdk

namespace mer::editor::mvp {
class PropertyRenderer;

class ObjectPropertyEntryBase : public ui::TreeElementBase {
public:
	explicit ObjectPropertyEntryBase(const std::shared_ptr<Gio::ListModel> &pChildren) : TreeElementBase(pChildren) {}

	virtual const std::string &getName() = 0;
};

class ObjectPropertyEntry : public ObjectPropertyEntryBase {
	sdk::Extension* extension;
	sdk::PropertyBase* nativeProperty;

protected:
	explicit ObjectPropertyEntry(sdk::PropertyBase* pNativeProperty, sdk::Extension* pExtension)
		: ObjectPropertyEntryBase(nullptr), extension(pExtension), nativeProperty(pNativeProperty) {}

public:
	static std::shared_ptr<ObjectPropertyEntry> create(sdk::PropertyBase* pProp,
													   sdk::Extension* pExtension) {
		return make_refptr_for_instance(new ObjectPropertyEntry(pProp, pExtension));
	}

	std::shared_ptr<Gio::MenuModel> getMenu() override { return nullptr; }

	[[nodiscard]] sdk::PropertyBase* getNativeProperty() const { return nativeProperty; }

	const std::string &getName() override;

	std::shared_ptr<PropertyRenderer> getRenderer() const;

	[[nodiscard]] sdk::Extension* getExtension() const { return extension; }
};

class ObjectExtensionEntry : public ObjectPropertyEntryBase {
	sdk::Extension* nativeExtension{};

protected:
	explicit ObjectExtensionEntry(sdk::Extension* pNativeExtension);

public:
	static std::shared_ptr<ObjectExtensionEntry> create(sdk::Extension* pNativeExtension) {
		return make_refptr_for_instance(new ObjectExtensionEntry(pNativeExtension));
	}

	std::shared_ptr<Gio::MenuModel> getMenu() override { return nullptr; }

	const std::string &getName() override;

	[[nodiscard]] sdk::Extension* getNativeExtension() const { return nativeExtension; }
};

} // namespace mer::editor::mvp

#endif //OBJECTPROPERTYENTRY_H
