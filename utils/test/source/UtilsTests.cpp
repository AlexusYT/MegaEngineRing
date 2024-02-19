//
// Created by alexus on 19.02.24.
//
#include <fstream>
#include <gtest/gtest.h>

#include "EngineUtils/utils/Utils.h"

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