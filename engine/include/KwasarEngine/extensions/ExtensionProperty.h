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
// Created by alexus on 28.09.24.
//

#ifndef EXTENSIONPROPERTY_H
#define EXTENSIONPROPERTY_H

#include "KwasarEngine/utils/ISerializable.h"
#include "KwasarEngine/utils/PropertyJsonSerializers.h"
#include "KwasarEngine/utils/Property.h"

namespace ke {
template <typename T>
class ExtensionProperty : public Property<T>, public ISerializable {
public:
	ExtensionProperty(IPropertyProvider* pProvider, const std::string &pName, const std::string &pDescription = {})
		: Property<T>(pProvider, pName, pDescription) {}

	ExtensionProperty &operator=(const T &pOther) {
		Property<T>::setValue(pOther);
		return *this;
	}

	ExtensionProperty &operator=(T &&pOther) {
		Property<T>::setValue(pOther);
		return *this;
	}

	void serialize(nlohmann::json & /*pJson*/, Extension* /*pExtension*/) override {
		//pJson[this->Property<T>::PropertyBase::getPropertyName()] =
		//	PropertyJsonSerializer<T>::serialize(Property<T>::getValue(), pExtension);
	}

	void deserialize(const nlohmann::json & /*pJson*/, Extension* /*pExtension*/) override {
		/*if (pJson.contains(this->Property<T>::getPropertyName()))
			Property<T>::setValue(PropertyJsonSerializer<T>::deserialize(
				pJson.at(this->Property<T>::getPropertyName()), pExtension));
		else
			Property<T>::setValue({});*/
	}
};
} // namespace ke

#endif //EXTENSIONPROPERTY_H
