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
// Created by alexus on 17.03.24.
//

#ifndef OBJECTPROPERTYENTRY_H
#define OBJECTPROPERTYENTRY_H
#include "EngineSDK/main/scene/objects/extensions/Extension.h"

class ObjectPropertyEntry : public Glib::Object {
	std::shared_ptr<mer::sdk::main::ExtensionPropertyBase> nativeProperty;

protected:
	explicit ObjectPropertyEntry(const std::shared_ptr<mer::sdk::main::ExtensionPropertyBase> &pNativeProperty)
		: nativeProperty(pNativeProperty) {}

public:
	static std::shared_ptr<ObjectPropertyEntry> create(
		const std::shared_ptr<mer::sdk::main::ExtensionPropertyBase> &pProp) {
		return Glib::make_refptr_for_instance(new ObjectPropertyEntry(pProp));
	}

	[[nodiscard]] const std::shared_ptr<mer::sdk::main::ExtensionPropertyBase> &getNativeProperty() const {
		return nativeProperty;
	}

	void setNativeProperty(const std::shared_ptr<mer::sdk::main::ExtensionPropertyBase> &pNativeProperty) {
		nativeProperty = pNativeProperty;
	}
};

#endif //OBJECTPROPERTYENTRY_H
