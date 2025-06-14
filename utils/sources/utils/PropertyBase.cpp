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
// Created by alexus on 28.10.24.
//

#include "EngineUtils/utils/PropertyBase.h"

#include "EngineUtils/utils/IPropertyProvider.h"

namespace mer::sdk {
PropertyBase::PropertyBase(IPropertyProvider* pProvider, const std::string &pName, const std::string &pDescription)
	: name(pName), description(pDescription), provider(pProvider) { if (provider) provider->addProperty(this); }

PropertyBase::~PropertyBase() { if (provider) provider->removeProperty(this); }

void PropertyBase::notifyChanged() { if (provider) provider->propertyChanged(this); }
} // namespace mer::sdk
