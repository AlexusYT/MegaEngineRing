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
// Created by alexus on 19.01.24.
//

#ifndef PROPERTY_H
#define PROPERTY_H
#include <sigc++/signal.h>

namespace mer::sdk {

/*
template<typename T>
class Property {
public:
	using PropertyT = T;
	using SignalT = sigc::signal<void()>;

private:
	T value;
	SignalT signalChanged;

public:
	Property() = default;

	Property(const Property &pOther) = delete;

	Property(Property &&pOther) noexcept : value(std::move(pOther.value)) {}

	Property(T &&pOther) noexcept {
		value = pOther;
		signalChanged();
	}

	Property &operator=(const Property &pOther) = delete;

	Property &operator=(Property &&pOther) noexcept {
		if (this == &pOther) return *this;
		value = std::move(pOther.value);
		return *this;
	}

	Property &operator=(const T &pOther) noexcept {
		signalChanged();
		value = pOther;
		return *this;
	}

	Property &operator=(T &&pOther) noexcept {
		signalChanged();
		value = pOther;
		return *this;
	}

	[[nodiscard]] const T &getValue() const { return value; }

	void setValue(const T &pValue) {
		signalChanged();
		value = pValue;
	}

	operator const T&() const { return value; }

	operator T&() { return value; }

	operator T() const { return value; }

	T* operator->() { return &value; }

	T &operator*() { return value; }

	void notifyChanged() const { signalChanged(); }

	[[nodiscard]] const SignalT &getSignalChanged() const { return signalChanged; }
};
*/

} // namespace mer::sdk

#endif //PROPERTY_H
