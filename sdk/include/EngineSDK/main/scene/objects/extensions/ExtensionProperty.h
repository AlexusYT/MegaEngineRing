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
// Created by alexus on 28.09.24.
//

#ifndef EXTENSIONPROPERTY_H
#define EXTENSIONPROPERTY_H

#include "EngineSDK/utils/ISerializable.h"
#include "EngineSDK/utils/PropertyJsonSerializers.h"
#include "EngineUtils/utils/Property.h"

namespace mer::sdk::main {

template<typename T>
class ExtensionProperty : public utils::Property<T>, public ISerializable {


public:
	ExtensionProperty(utils::IPropertyProvider* pProvider, const std::string &pName,
					  const std::string &pDescription = {})
		: utils::Property<T>(pProvider, pName, pDescription) {}

	ExtensionProperty &operator=(const T &pOther) {
		utils::Property<T>::setValue(pOther);
		return *this;
	}

	ExtensionProperty &operator=(T &&pOther) {
		utils::Property<T>::setValue(pOther);
		return *this;
	}

	void serialize(nlohmann::json &pJson, Extension* pExtension) override {
		pJson[this->utils::Property<T>::PropertyBase::getPropertyName()] =
			utils::PropertyJsonSerializer<T>::serialize(utils::Property<T>::getValue(), pExtension);
	}

	void deserialize(const nlohmann::json &pJson, Extension* pExtension) override {
		if (pJson.contains(this->utils::Property<T>::getPropertyName()))
			utils::Property<T>::setValue(utils::PropertyJsonSerializer<T>::deserialize(
				pJson.at(this->utils::Property<T>::getPropertyName()), pExtension));
		else
			utils::Property<T>::setValue({});
	}
};


} // namespace mer::sdk::main

#endif //EXTENSIONPROPERTY_H
