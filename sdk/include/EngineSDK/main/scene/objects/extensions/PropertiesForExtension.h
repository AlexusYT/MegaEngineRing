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
// Created by alexus on 28.06.24.
//

#ifndef PROPERTIESFOREXTENSION_H
#define PROPERTIESFOREXTENSION_H

#include "EngineSDK/main/scene/objects/extensions/ExtensionProperties.h"

namespace mer::sdk::main {

#define DECLARE_PROPERTY(__Type__, __Name__)                                                                           \
	ValueChangedArgs<const __Type__ &> on##__Name__##Changed{};                                                        \
	__Type__ property##__Name__

#define DECLARE_PROPERTY_DEF(__Type__, __Name__, __DefaultValue__)                                                     \
	__Type__ property##__Name__{__DefaultValue__};                                                                     \
	ValueChangedArgs<const __Type__ &> on##__Name__##Changed {}

#define ADD_PROPERTY_EVENT(__ClassName__, __Name__, __Title__, __Description__)                                        \
	ExtensionProperty<decltype(property##__Name__)>* extProperty##__Name__ =                                           \
		dynamic_cast<ExtensionProperty<decltype(property##__Name__)>*>(                                                \
			getProps()                                                                                                 \
				.emplace_back(this, #__Name__, __Description__, &__ClassName__::get##__Name__, {},                     \
							  &__ClassName__::getOn##__Name__##Changed)                                                \
				->setGetterName("get" #__Name__)                                                                       \
				->setEventName("getOn" #__Name__ "Changed"))

#define ADD_PROPERTY_SET_EVENT(__ClassName__, __Name__, __Title__, __Description__)                                    \
	ExtensionProperty<decltype(property##__Name__)>* extProperty##__Name__ =                                           \
		dynamic_cast<ExtensionProperty<decltype(property##__Name__)>*>(                                                \
			getProps()                                                                                                 \
				.emplace_back(this, #__Name__, __Description__, &__ClassName__::get##__Name__,                         \
							  &__ClassName__::set##__Name__, &__ClassName__::getOn##__Name__##Changed)                 \
				->setGetterName("get" #__Name__)                                                                       \
				->setSetterName("set" #__Name__)                                                                       \
				->setEventName("getOn" #__Name__ "Changed"))

/**
 * @brief Class that manages the properties.
 *
 * To create the property, use
 * @code  DECLARE_PROPERTY(<TypeName>, <VarName>); @endcode
 *
 * To register the property with getter and event, use
 * @code ADD_PROPERTY_EVENT(<ClassName>, <VarName>, <Title>, <Description>); @endcode
 *
 * To register the property with getter, setter and event, use
 * @code ADD_PROPERTY_SET_EVENT(<ClassName>, <VarName>, <Title>, <Description>); @endcode
 */
class PropertiesForExtension {


public:
	ExtensionProperties properties;
	template<typename... Args>
	using ValueChangedArgs = sigc::signal<void(Args...)>;

	[[nodiscard]] const ExtensionProperties &getProperties() const { return properties; }

protected:
	[[nodiscard]] ExtensionProperties &getProps() { return properties; }
};
} // namespace mer::sdk::main

#endif //PROPERTIESFOREXTENSION_H
