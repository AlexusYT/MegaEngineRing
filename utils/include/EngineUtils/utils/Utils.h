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
// Created by alexus on 31.12.23.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <typeinfo>

class Utils {
private:
public:
	template<typename T>
	static std::string getTypeName(const T &pE) {
		return demangle(typeid(pE).name());
	}

	template<typename T>
	static std::string getTypeName(T* pE) {
		return demangle(typeid(*pE).name());
	}

	template<typename T>
	static std::string getTypeName() {
		std::string result;
		if constexpr (std::is_const_v<std::remove_reference_t<T>>) result += "const ";
		if constexpr (std::is_pointer<T>()) {
			result += demangle(typeid(std::remove_pointer_t<T>).name());

		} else
			result += demangle(typeid(T).name());
		if constexpr (std::is_pointer_v<T>) result += "*";
		if constexpr (std::is_reference_v<T>) result += " &";
		return result;
	}

	static std::string demangle(const std::string &pMangledName);

	static std::string parseDlError(const std::string &pMsg);
};


#endif //UTILS_H
