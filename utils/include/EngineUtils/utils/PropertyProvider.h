//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 03.01.25.
//

#ifndef PROPERTYPROVIDER_H
#define PROPERTYPROVIDER_H
#include <string>
#include <unordered_map>

#include "IPropertyProvider.h"

namespace mer::sdk::utils {

class PropertyProvider : public IPropertyProvider {
	std::unordered_map<std::string, PropertyBase*> properties;

protected:
	void addProperty(PropertyBase* pProperty) override;

	void removeProperty(PropertyBase* pProperty) override;

	void propertyChanged(PropertyBase* pProperty) override;

public:
	[[nodiscard]] const std::unordered_map<std::string, PropertyBase*> &getProperties() const { return properties; }

	[[nodiscard]] std::unordered_map<std::string, PropertyBase*> &getProperties() { return properties; }

	PropertyBase* getProperty(const std::string &pName);

	PropertyBase* getProperty(const std::string &pName) const;

	template<typename T>
	T* getProperty(const std::string &pName) {
		return dynamic_cast<T*>(getProperty(pName));
	}

	template<typename T>
	T* getProperty(const std::string &pName) const {
		return dynamic_cast<T*>(getProperty(pName));
	}
};


} // namespace mer::sdk::utils

#endif //PROPERTYPROVIDER_H
