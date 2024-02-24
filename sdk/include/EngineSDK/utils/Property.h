//
// Created by alexus on 19.01.24.
//

#ifndef PROPERTY_H
#define PROPERTY_H
#include <sigc++/signal.h>

namespace mer::sdk::utils {

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

} // namespace mer::sdk::utils

#endif //PROPERTY_H
