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

#include <EngineUtils/utils/Utils.h>
#include <cxxabi.h>
#include <regex>

void replace_name(std::string &pIn, const std::string &pWhat, const std::string &pReplaceWith) {
	std::regex regex(pWhat);
	pIn = std::regex_replace(pIn, regex, pReplaceWith);
	/*if (auto iter = pIn.find(pWhat); iter != std::string::npos) {
		//
		pIn.replace(iter, pWhat.size(), pReplaceWith);
	}*/
}

std::string Utils::demangle(const std::string &pMangledName) {
	if (const auto str = abi::__cxa_demangle(pMangledName.c_str(), nullptr, nullptr, nullptr)) {
		try {
			std::string name = str;

			replace_name(name, "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
						 "std::string");
			return name;
		} catch (...) { return str; }
	}
	return pMangledName;
}

std::string Utils::parseDlError(const std::string &pMsg) {

	if (auto iter = pMsg.find("undefined symbol: "); iter != std::string::npos) {
		return "undefined symbol: " + demangle(pMsg.substr(iter + 18));
	}
	return pMsg;
}