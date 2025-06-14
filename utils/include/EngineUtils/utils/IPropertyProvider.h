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
// Created by alexus on 28.06.24.
//

#ifndef PROPERTIESFOREXTENSION_H
#define PROPERTIESFOREXTENSION_H

namespace mer::sdk {
class PropertyBase;

/**
 * @brief Class that manages the properties.
 *
 * To create the property, use
 * @code
 * ExtensionProperty<{TypeName}> property{VarName};
 * @endcode
 * Register the property in class constructor.
 * @code
 * ClassName::ClassName() : property{VarName}(this, "{VarName}") {}
 * @endcode
 * If you need to initialize the default value, do this in constructor
 * @code
 * ClassName::ClassName() : property{VarName}(this, "{VarName}") {
 *		property{VarName} = {Value};
 * }
 * @endcode
 *
 * @note
 * The variable name must starts with `property` and the variable must be public. These requirements are used for script
 * generation.
 *
 */
class IPropertyProvider {
public:
	virtual ~IPropertyProvider() = default;

	virtual void addProperty(PropertyBase* pProperty) = 0;

	virtual void removeProperty(PropertyBase* pProperty) = 0;

	virtual void propertyChanged(PropertyBase* pProperty) = 0;
};
} // namespace mer::sdk

#endif //PROPERTIESFOREXTENSION_H
