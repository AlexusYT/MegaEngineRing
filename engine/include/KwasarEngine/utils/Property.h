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
// Created by alexus on 28.10.24.
//

#ifndef PROPERTY_H
#define PROPERTY_H
#include <iostream>
#include <sigc++/signal.h>

#include "KwasarEngine/utils/PropertyBase.h"
#include "PropertyNotifiable.h"

namespace ke {
class IPropertyNotifiable;
}

namespace ke {
template <typename T>
concept IsSharedPtr = requires { std::is_pointer_v<typename T::element_type>; };

template <typename T>
class PropertyReadOnly {
	const T &value;
	sigc::signal<void(const T &)> &valueChanged;
	const sigc::slot<T()> &getter;

public:
	using PropertyT = T;

	PropertyReadOnly() = delete;

	PropertyReadOnly(const T &pValue, sigc::signal<void(const T &)> &pValueChanged, const sigc::slot<T()> &pGetter)
		: value(pValue), valueChanged(pValueChanged), getter(pGetter) {}

	const T &getValue() const { return value; }

	operator const T &() const { return value; }

	const T &operator*() const { return value; }

	bool operator==(const T &pRhs) { return value == pRhs; }

	bool operator!=(const T &pRhs) { return !(value == pRhs); }

	auto operator->() const requires IsSharedPtr<T> { return value.get(); }

	auto operator->() const requires requires { requires !IsSharedPtr<T>; } { return &value; }

	template <typename T1>
	T1 operator+(const T1 &pRhs) const { return value + pRhs; }

	[[nodiscard]] sigc::slot<T()> &getGetter() const { return getter; }

	[[nodiscard]] const sigc::signal<void(const T &)> &getEvent() const { return valueChanged; }

	sigc::connection connectEvent(const sigc::slot<void(const T &)> &pSlot) const {
		pSlot(value);
		return valueChanged.connect(pSlot);
	}
};

template <typename T>
concept IsConvertibleToNotify = std::convertible_to<typename T::element_type*, IPropertyNotifiable*> ||
								std::convertible_to<T, IPropertyNotifiable*> ||
								std::is_convertible_v<std::add_lvalue_reference_t<T>, IPropertyNotifiable &>;
template <typename T>
concept HasBaseClassNotify = std::is_base_of_v<IPropertyNotifiable, typename T::element_type> ||
							 std::is_base_of_v<IPropertyNotifiable, std::remove_pointer_t<T>>;

template <typename T>
class Property : public PropertyBase {
	T value{};
	sigc::signal<void(const T &)> valueChanged;
	sigc::signal<T(const T &)> valueChanging;
	sigc::slot<T()> getter;
	sigc::slot<void(const T &)> setter;

public:
	using PropertyT = T;

	Property(const Property &pOther) = delete;

	Property(Property &&pOther) noexcept
		: PropertyBase(std::move(pOther)),
		  value(std::move(pOther.value)),
		  valueChanged(std::move(pOther.valueChanged)),
		  valueChanging(std::move(pOther.valueChanging)),
		  getter(std::move(pOther.getter)),
		  setter(std::move(pOther.setter)) {}

	Property &operator=(const Property &pOther) = delete;

	Property &operator=(Property &&pOther) noexcept {
		if (this == &pOther) return *this;
		PropertyBase::operator=(std::move(pOther));
		value = std::move(pOther.value);
		valueChanged = std::move(pOther.valueChanged);
		valueChanging = std::move(pOther.valueChanging);
		getter = std::move(pOther.getter);
		setter = std::move(pOther.setter);
		return *this;
	}

	Property() = delete;

	Property(IPropertyProvider* pProvider, const std::string &pName, const std::string &pDescription = {})
		: PropertyBase(pProvider, pName, pDescription) {
		getter = sigc::mem_fun(*this, &Property::getValue);
		setter = sigc::mem_fun(*this, &Property::setValue);
	}

	const T &getValue() const { return value; }

	operator const T &() const { return value; }

	const T &operator*() const { return value; }

	T &operator*() { return value; }

	void setValue(const T &pOther) {
		if (valueChanging.empty()) value = pOther;
		else
			value = valueChanging(pOther);

		if constexpr (!IsConvertibleToNotify<T> && HasBaseClassNotify<T>) {
			static_assert(IsConvertibleToNotify<T>,
						  "Property value class has IPropertyNotifiable base class, but it is not accessible. "
						  "IPropertyNotifiable must be publicly inherited");
		}
		if constexpr (IsConvertibleToNotify<T>) {
			if constexpr (std::__is_shared_ptr<T>) {
				if (const auto notifiable = dynamic_cast<IPropertyNotifiable*>(value.get())) notifiable->setBase(this);
			} else if constexpr (std::is_pointer_v<T>) {
				if (auto notifiable = dynamic_cast<IPropertyNotifiable*>(value)) notifiable->setBase(this);
			} else if constexpr (std::is_class_v<T>) {
				auto &notifiable = dynamic_cast<IPropertyNotifiable &>(value);
				notifiable.setBase(this);
			}
		}

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

	auto operator->() const requires IsSharedPtr<T> { return value.get(); }

	auto operator->() requires IsSharedPtr<T> { return value.get(); }

	auto operator->() const requires requires { requires !IsSharedPtr<T>; } { return &value; }

	auto operator->() requires requires { requires !IsSharedPtr<T>; } { return &value; }

	template <typename T1>
	T1 operator+(const T1 &pRhs) const { return value + pRhs; }

	[[nodiscard]] sigc::slot<T()> &getGetter() { return getter; }

	[[nodiscard]] sigc::slot<void(const T &)> getSetter() { return setter; }

	[[nodiscard]] sigc::signal<void(const T &)> &getEvent() { return valueChanged; }

	sigc::connection connectEvent(const sigc::slot<void(const T &)> &pSlot) {
		pSlot(value);
		return valueChanged.connect(pSlot);
	}

	[[nodiscard]] sigc::signal<T(const T &)> &getReturnOverride() { return valueChanging; }

	void notifyChanged() override {
		valueChanged(value);
		PropertyBase::notifyChanged();
	}

	PropertyReadOnly<T> getReadOnly() { return PropertyReadOnly<T>(value, valueChanged, getter); }
};
} // namespace ke

#endif //PROPERTY_H
