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
// Created by alexus on 19.02.24.
//
#include <fstream>
#include <gtest/gtest.h>

#include "KwasarEngine/utils/Utils.h"

TEST(UtilsTests, demangleUnknownName) { ASSERT_EQ(Utils::demangle("sssss"), "sssss"); }

TEST(UtilsTests, demangleSimpleName) {
	std::string mangledName = typeid(int).name();
	EXPECT_NE(mangledName, "int");
	ASSERT_EQ(Utils::demangle(mangledName), "int");
}

TEST(UtilsTests, demangleComplexName) {
	std::string mangledName = typeid(std::string).name();
	EXPECT_NE(mangledName, "std::string");
	ASSERT_EQ(Utils::demangle(mangledName), "std::string");
}

TEST(UtilsTests, demangleUnknownComplexName) {
	std::string mangledName = typeid(std::ifstream).name();
	ASSERT_EQ(Utils::demangle(mangledName), "std::basic_ifstream<char, std::char_traits<char> >");
}

TEST(UtilsTests, getTypeNameFromArg) {
	auto run = std::runtime_error("");
	std::exception &e = run;
	EXPECT_EQ(Utils::getTypeName(&e), "std::runtime_error");
	EXPECT_EQ(Utils::getTypeName(e), "std::runtime_error");
}

TEST(UtilsTests, getTypeNameFromType) {
	EXPECT_EQ(Utils::getTypeName<const int &>(), "const int &");
	EXPECT_EQ(Utils::getTypeName<int*>(), "int*");
	EXPECT_EQ(Utils::getTypeName<int*&>(), "int* &");
}

TEST(UtilsTests, parseDlErrorUnknown) {
	const std::string data = "some error";
	EXPECT_EQ(Utils::parseDlError(data), data);
}

TEST(UtilsTests, parseDlError) {
	const std::string data = std::format("undefined symbol: {}", typeid(Utils::parseDlError).name());
	EXPECT_EQ(Utils::parseDlError(data), "undefined symbol: std::string (std::string const&)");
}
