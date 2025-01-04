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

#include "EngineUtils/utils/PropertyProvider.h"

#include "EngineUtils/utils/PropertyBase.h"

namespace mer::sdk::utils {
void PropertyProvider::addProperty(PropertyBase* pProperty) {
	properties.emplace(pProperty->getPropertyName(), pProperty);
}

void PropertyProvider::removeProperty(PropertyBase* pProperty) { properties.erase(pProperty->getPropertyName()); }

void PropertyProvider::propertyChanged(PropertyBase* /*pProperty*/) {}

PropertyBase* PropertyProvider::getProperty(const std::string &pName) {
	const auto it = properties.find(pName);
	if (it == properties.end()) return nullptr;
	return it->second;
}

PropertyBase* PropertyProvider::getProperty(const std::string &pName) const {
	const auto it = properties.find(pName);
	if (it == properties.end()) return nullptr;
	return it->second;
}
} // namespace mer::sdk::utils