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
// Created by alexus on 28.10.24.
//

#ifndef PROPERTY_H
#define PROPERTY_H
#include <sigc++/signal.h>

#include "PropertyBase.h"

namespace mer::sdk::utils {

template<typename T>
class Property : public PropertyBase {
	T value{};
	sigc::signal<void(const T &)> valueChanged;
	sigc::signal<T(const T &)> valueChanging;


public:
	using PropertyT = T;

	Property() = delete;

	Property(IPropertyProvider* pProvider, const std::string &pName, const std::string &pDescription = {})
		: PropertyBase(pProvider, pName, pDescription) {}

	const T &getValue() const { return value; }

	//operator T() { return getValue(); }

	operator const T&() const { return value; }

	const T &operator*() const { return value; }

	T &operator*() { return value; }

	void setValue(const T &pOther) {
		if (valueChanging.empty()) value = pOther;
		else
			value = valueChanging(pOther);
		notifyChanged();
	}

	virtual Property &operator=(const T &pOther) {
		setValue(pOther);
		return *this;
	}

	virtual Property &operator=(T &&pOther) {
		setValue(pOther);
		return *this;
	}

	bool operator==(const T &pRhs) { return value == pRhs; }

	bool operator!=(const T &pRhs) { return !(value == pRhs); }

	auto operator->()
		requires requires { std::is_pointer_v<typename T::element_type>; }
	{
		return value.get();
	}

	/*
	template<typename U>
	T* operator->()
		requires requires { !std::is_same_v<T, std::shared_ptr<U>>; }
	{
		return &value;
	}*/

	template<typename T1>
	T1 operator+(const T1 &pRhs) const {
		return value + pRhs;
	}

	[[nodiscard]] sigc::slot<T()> &getGetter() { return sigc::mem_fun(*this, &Property::getValue); }

	[[nodiscard]] sigc::slot<void(const T &)> getSetter() {
		return sigc::slot<void(const T &)>(sigc::mem_fun(*this, &Property::setValue));
	}

	[[nodiscard]] sigc::signal<void(const T &)> &getEvent() { return valueChanged; }

	sigc::connection connectEvent(const sigc::slot<void(const T &)> &pSlot) {
		pSlot(value);
		return valueChanged.connect(pSlot);
	}

	[[nodiscard]] sigc::signal<T(const T &)> &getReturnOverride() { return valueChanging; }

	void notifyChanged() {
		notifyPropertyChanged();
		valueChanged(value);
	}
};

} // namespace mer::sdk::utils

#endif //PROPERTY_H
