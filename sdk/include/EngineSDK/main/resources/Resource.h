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
// Created by alexus on 05.10.24.
//

#ifndef RESOURCE_H
#define RESOURCE_H
#include "EngineUtils/utils/IPropertyProvider.h"
#include "IResource.h"

namespace mer::sdk::main {

class Resource : public IResource, public utils::IPropertyProvider {
	std::vector<utils::PropertyBase*> properties;
	std::filesystem::path resourceUri;

public:
	void addProperty(utils::PropertyBase* pProperty) override;

	void removeProperty(utils::PropertyBase* pProperty) override;

	void propertyChanged(utils::PropertyBase* pProperty) override;

	const std::filesystem::path &getResourceUri() override { return resourceUri; }

	[[nodiscard]] std::string getResourceName() const override { return resourceUri.stem(); }

	void setResourceUri(const std::filesystem::path &pResourceUri) override { resourceUri = pResourceUri; }
};

} // namespace mer::sdk::main

#endif //RESOURCE_H
