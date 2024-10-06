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
// Created by alexus on 22.03.24.
//

#ifndef EXTENSIONPROPERTYBASE_H
#define EXTENSIONPROPERTYBASE_H

#include "IPropertyProvider.h"

namespace mer::sdk::main {

class ExtensionPropertyBase {
	std::string name;
	std::string description;

protected:
	ExtensionPropertyBase() = default;

	ExtensionPropertyBase(const std::string &pName, const std::string &pDescription)
		: name(pName), description(pDescription) {}

	virtual ~ExtensionPropertyBase() = default;

public:
	[[nodiscard]] const std::string &getPropertyName() const { return name; }

	void setPropertyName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getPropertyDescription() const { return description; }

	void setPropertyDescription(const std::string &pDescription) { description = pDescription; }

	virtual void serialize(nlohmann::json &pJson, Extension* pExtension);

	virtual void deserialize(const nlohmann::json &pJson, Extension* pExtension);
};

inline void ExtensionPropertyBase::serialize(nlohmann::json & /*pJson*/, Extension* /*pExtension*/) {}

inline void ExtensionPropertyBase::deserialize(const nlohmann::json & /*pJson*/, Extension* /*pExtension*/) {}
} // namespace mer::sdk::main


#endif //EXTENSIONPROPERTYBASE_H
