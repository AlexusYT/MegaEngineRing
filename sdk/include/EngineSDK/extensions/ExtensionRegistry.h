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
// Created by alexus on 15.03.24.
//

#ifndef EXTENSIONREGISTRY_H
#define EXTENSIONREGISTRY_H
#include <functional>
#include <typeindex>

#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::sdk {
class Extension;

struct ExtensionClassInfo {
	std::string typeName;
	std::function<std::shared_ptr<Extension>()> createFunc;
};

template<typename ClassT>
concept ExtensionTypeName = requires {
	{ ClassT::typeName() } -> std::same_as<const char*>;
};

class ExtensionRegistry {

	inline static std::unordered_map<std::string, ExtensionClassInfo> extensions;
	inline static std::unordered_map<std::type_index, ExtensionClassInfo> extensionByType;

public:
	static void init();

	static void deinit();

	template<typename ClassT>
	static void registerClass() {
		static_assert(ExtensionTypeName<ClassT>,
					  "ClassT::typeName() must be declared as public static and return non null string");
		ExtensionClassInfo info;
		info.typeName = ClassT::typeName();
		info.createFunc = ClassT::create;
		extensions.emplace(info.typeName, info);
		extensionByType.emplace(typeid(ClassT), info);
	}

	[[nodiscard]] static const std::unordered_map<std::string, ExtensionClassInfo> &getExtensions() {
		return extensions;
	}

	static std::shared_ptr<Extension> newInstance(const std::string &pTypeName);

	template<typename ClassT>
	static std::shared_ptr<ClassT> newInstance() {
		if (auto ptr = newInstance(ClassT::getTypeName())) return std::dynamic_pointer_cast<ClassT>(ptr);
		return nullptr;
	}

	static ReportMessagePtr getTypeNameFor(Extension* pExt, std::string &pNameOut);
};
} // namespace mer::sdk


#endif //EXTENSIONREGISTRY_H
