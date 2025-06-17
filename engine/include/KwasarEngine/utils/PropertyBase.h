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
// Created by alexus on 22.03.24.
//

#ifndef EXTENSIONPROPERTYBASE_H
#define EXTENSIONPROPERTYBASE_H
#include <string>

namespace ke {
class IPropertyProvider;

class PropertyBase {
	friend class PropertyNotifiable;
	std::string name;
	std::string description;
	IPropertyProvider* provider;

protected:
	PropertyBase(IPropertyProvider* pProvider, const std::string &pName, const std::string &pDescription);

	virtual ~PropertyBase();

public:
	[[nodiscard]] const std::string &getPropertyName() const { return name; }

	void setPropertyName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getPropertyDescription() const { return description; }

	void setPropertyDescription(const std::string &pDescription) { description = pDescription; }

	/**
	 * @brief Send notification to Property::valueChanged signal and IPropertyProvider::propertyChanged method.
	 */
	virtual void notifyChanged();
};
} // namespace ke


#endif //EXTENSIONPROPERTYBASE_H
