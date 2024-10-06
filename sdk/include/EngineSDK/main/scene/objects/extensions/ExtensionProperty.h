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
#include <sigc++/signal.h>

#include "EngineSDK/utils/PropertyJsonSerializers.h"
#include "ExtensionPropertyBase.h"

namespace mer::sdk::main {

template<typename T>
class ExtensionProperty : public ExtensionPropertyBase {
	IPropertyProvider* provider;
	T value{};
	sigc::signal<void(const T &)> valueChanged;
	sigc::signal<T(const T &)> valueChanging;


public:
	using PropertyT = T;

	ExtensionProperty(IPropertyProvider* pProvider, const std::string &pName, const std::string &pDescription = {})
		: ExtensionPropertyBase(pName, pDescription), provider(pProvider) {
		pProvider->addProperty(this);
	}

	const T &getValue() const { return value; }

	//operator T() { return getValue(); }

	operator const T&() const { return value; }

	const T &operator*() const { return value; }

	T &operator*() { return value; }

	void setValue(const T &pOther) {
		if (valueChanging.empty()) value = pOther;
		else
			value = valueChanging(pOther);
		provider->propertyChanged(this);
		valueChanged(value);
	}

	ExtensionProperty &operator=(const T &pOther) {
		setValue(pOther);
		return *this;
	}

	ExtensionProperty &operator=(T &&pOther) {
		setValue(pOther);
		return *this;
	}

	T &operator->() { return value; }

	bool operator==(const T &pRhs) { return value == pRhs; }

	bool operator!=(const T &pRhs) { return !(value == pRhs); }

	template<typename T1>
	T1 operator+(const T1 &pRhs) const {
		return value + pRhs;
	}

	[[nodiscard]] sigc::slot<T()> &getGetter() { return sigc::mem_fun(*this, &ExtensionProperty::getValue); }

	[[nodiscard]] sigc::slot<void(const T &)> getSetter() {
		return sigc::slot<void(const T &)>(sigc::mem_fun(*this, &ExtensionProperty::setValue));
	}

	[[nodiscard]] sigc::signal<void(const T &)> &getEvent() { return valueChanged; }

	[[nodiscard]] sigc::signal<T(const T &)> &getReturnOverride() { return valueChanging; }

	void serialize(nlohmann::json &pJson, Extension* pExtension) override;

	void deserialize(const nlohmann::json &pJson, Extension* pExtension) override;
};

template<typename T>
void ExtensionProperty<T>::serialize(nlohmann::json &pJson, Extension* pExtension) {
	pJson[getPropertyName()] = utils::PropertyJsonSerializer<T>::serialize(value, pExtension);
}

template<typename T>
void ExtensionProperty<T>::deserialize(const nlohmann::json &pJson, Extension* pExtension) {
	value = utils::PropertyJsonSerializer<T>::deserialize(pJson.at(getPropertyName()), pExtension);
}


} // namespace mer::sdk::main

#endif //EXTENSIONPROPERTY_H
