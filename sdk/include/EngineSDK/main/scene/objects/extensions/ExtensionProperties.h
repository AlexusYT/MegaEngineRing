// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 22.03.24.
//

#ifndef EXTENSIONPROPERTIES_H
#define EXTENSIONPROPERTIES_H


#include <sigc++/signal.h>
#include <vector>

namespace mer::sdk::main {
class ExtensionPropertyBase {
	std::string name;
	std::string description;
	std::string getterName;
	std::string setterName;
	std::string eventName;

protected:
	ExtensionPropertyBase() = default;

	ExtensionPropertyBase(const std::string &pName, const std::string &pDescription)
		: name(pName), description(pDescription) {}

	virtual ~ExtensionPropertyBase() = default;

public:
	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getDescription() const { return description; }

	void setDescription(const std::string &pDescription) { description = pDescription; }

	[[nodiscard]] const std::string &getGetterName() const { return getterName; }

	ExtensionPropertyBase* setGetterName(const std::string &pGetterName) {
		getterName = pGetterName;
		return this;
	}

	[[nodiscard]] const std::string &getSetterName() const { return setterName; }

	ExtensionPropertyBase* setSetterName(const std::string &pSetterName) {
		setterName = pSetterName;
		return this;
	}

	[[nodiscard]] const std::string &getEventName() const { return eventName; }

	ExtensionPropertyBase* setEventName(const std::string &pEventName) {
		eventName = pEventName;
		return this;
	}
};

class ExtensionPropertyGroup : public ExtensionPropertyBase {
	std::vector<std::shared_ptr<ExtensionPropertyBase>> children;

public:
	ExtensionPropertyGroup() = default;

	ExtensionPropertyGroup(const std::string &pName, const std::string &pDescription)
		: ExtensionPropertyBase(pName, pDescription) {}

	[[nodiscard]] const std::vector<std::shared_ptr<ExtensionPropertyBase>> &getChildren() const { return children; }

	void addChild(const std::shared_ptr<ExtensionPropertyBase> &pChild) { children.emplace_back(pChild); }
};

template<typename T>
class ExtensionProperty : public ExtensionPropertyBase {

	sigc::slot<T()> getterFunc;
	sigc::slot<void(const T &)> setterFunc;
	sigc::slot<sigc::signal<void(const T &)>()> eventFunc;


public:
	using PropertyT = T;
	ExtensionProperty() = default;

	ExtensionProperty(const std::string &pName, const std::string &pDescription, const sigc::slot<T()> &pGetterFunc,
					  const sigc::slot<void(const T &)> &pSetterFunc)
		: ExtensionPropertyBase(pName, pDescription), getterFunc(pGetterFunc), setterFunc(pSetterFunc) {}

	ExtensionProperty(const std::string &pName, const std::string &pDescription, const sigc::slot<T()> &pGetterFunc,
					  const sigc::slot<void(const T &)> &pSetterFunc,
					  const sigc::slot<sigc::signal<void(const T &)>()> &pEventFunc)
		: ExtensionPropertyBase(pName, pDescription), getterFunc(pGetterFunc), setterFunc(pSetterFunc),
		  eventFunc(pEventFunc) {}

	template<typename ClassT>
	ExtensionProperty(ClassT* pClass, const std::string &pName, const std::string &pDescription,
					  T (ClassT::*pGetterFunc)() const, void (ClassT::*pSetterFunc)(T))
		: ExtensionPropertyBase(pName, pDescription) {
		getterFunc = sigc::mem_fun<T, ClassT>(*dynamic_cast<ClassT*>(pClass), pGetterFunc);
		setterFunc = sigc::mem_fun<void, ClassT>(*dynamic_cast<ClassT*>(pClass), pSetterFunc);
	}

	template<typename ClassT>
	ExtensionProperty(ClassT* pClass, const std::string &pName, const std::string &pDescription,
					  T (ClassT::*pGetterFunc)() const, void (ClassT::*pSetterFunc)(T),
					  sigc::signal<void(T)> (ClassT::*pEventFunc)() const)
		: ExtensionPropertyBase(pName, pDescription) {
		getterFunc = sigc::mem_fun<T, ClassT>(*dynamic_cast<ClassT*>(pClass), pGetterFunc);
		setterFunc = sigc::mem_fun<void, ClassT>(*dynamic_cast<ClassT*>(pClass), pSetterFunc);
		eventFunc = sigc::mem_fun<sigc::signal<void(T)>, ClassT>(*dynamic_cast<ClassT*>(pClass), pEventFunc);
	}

	[[nodiscard]] const sigc::slot<T()> &getGetterFunc() const { return getterFunc; }

	void setGetterFunc(const sigc::slot<T()> &pGetterFunc) { getterFunc = pGetterFunc; }

	[[nodiscard]] const sigc::slot<void(const T &)> &getSetterFunc() const { return setterFunc; }

	void setSetterFunc(const sigc::slot<void(const T &)> &pSetterFunc) { setterFunc = pSetterFunc; }

	[[nodiscard]] const sigc::slot<sigc::signal<void(const T &)>()> &getEventFunc() const { return eventFunc; }

	void setEventFunc(const sigc::slot<sigc::signal<void(const T &)>()> &pEventFunc) { eventFunc = pEventFunc; }
};

class ExtensionProperties : public std::vector<std::shared_ptr<ExtensionPropertyBase>> {

public:
	template<typename T, typename T1 = std::remove_cvref_t<T>, typename ClassT>
	// ReSharper disable once CppInconsistentNaming
	ExtensionProperty<T1>* emplace_back(ClassT* pClass, const std::string &pName, const std::string &pDescription,
										T (ClassT::*pGetterFunc)() const, void (ClassT::*pSetterFunc)(T)) {
		auto clazz = dynamic_cast<ClassT*>(pClass);

		auto getter = sigc::mem_fun<T, ClassT>(*clazz, pGetterFunc);
		auto setter = sigc::mem_fun<void, ClassT>(*clazz, pSetterFunc);

		auto prop = std::make_shared<ExtensionProperty<T1>>(pName, pDescription, getter, setter);
		vector::emplace_back(prop);
		return dynamic_cast<ExtensionProperty<T1>*>(prop.get());
	}

	template<typename T, typename T1 = std::remove_cvref_t<T>, typename ClassT>
	// ReSharper disable once CppInconsistentNaming
	ExtensionProperty<T1>* emplace_back(ClassT* pClass, const std::string &pName, const std::string &pDescription,
										T (ClassT::*pGetterFunc)() const, void (ClassT::*pSetterFunc)(T),
										sigc::signal<void(const T1 &)> &(ClassT::*pEventFunc)()) {
		auto clazz = dynamic_cast<ClassT*>(pClass);

		auto getter = sigc::mem_fun<T, ClassT>(*clazz, pGetterFunc);
		auto setter = sigc::mem_fun<void, ClassT>(*clazz, pSetterFunc);
		auto event = sigc::mem_fun<sigc::signal<void(const T1 &)> &, ClassT>(*clazz, pEventFunc);

		auto prop = std::make_shared<ExtensionProperty<T1>>(pName, pDescription, getter, setter, event);
		vector::emplace_back(prop);
		return dynamic_cast<ExtensionProperty<T1>*>(prop.get());
	}
};
} // namespace mer::sdk::main


#endif //EXTENSIONPROPERTIES_H
