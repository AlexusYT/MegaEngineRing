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
#include <gtest/gtest.h>
#include <regex>

#include "KwasarEngine/utils/Logger.h"
using namespace ke;

class LoggerTestsFixture : public testing::Test {
	bool useColors = true;
	bool useCErr = false;

protected:
	void SetUp() override {
		useColors = Logger::isUseColors();
		useCErr = Logger::isUseCErr();
	}

	void TearDown() override {
		Logger::setUseColors(useColors);
		Logger::setUseCErr(useCErr);
	}
};

std::string capture_err(const std::string &pMsg) {
	testing::internal::CaptureStderr();
	Logger::error(pMsg);
	return testing::internal::GetCapturedStderr();
}

std::string capture_out(const std::string &pMsg) {
	testing::internal::CaptureStdout();
	Logger::error(pMsg);
	return testing::internal::GetCapturedStdout();
}

void check_msg(const std::string &pMsg, const std::string &pRegex) {
	EXPECT_TRUE(std::regex_match(pMsg, std::regex(pRegex))) << "Actual message: " << pMsg;
}

TEST_F(LoggerTestsFixture, LogErrorColored) {
	//language=regexp
	check_msg(capture_out("Test"), R"(^\[31m\d\d:\d\d:\d\d.\d*[+\-]\d* - E - LoggerTests.cpp:\d*: Test\[0m\n$)");
}

TEST_F(LoggerTestsFixture, LogErrorSimple) {
	Logger::setUseColors(false);
	//language=regexp
	check_msg(capture_out("Test"), R"(^\d\d:\d\d:\d\d.\d*[+\-]\d* - E - LoggerTests.cpp:\d*: Test\n$)");
}

TEST_F(LoggerTestsFixture, LogErrorColoredCErr) {
	Logger::setUseCErr(true);
	//language=regexp
	check_msg(capture_err("Test"), R"(^\[31m\d\d:\d\d:\d\d.\d*[+\-]\d* - E - LoggerTests.cpp:\d*: Test\[0m\n$)");
}

TEST_F(LoggerTestsFixture, LogErrorSimpleCErr) {
	Logger::setUseCErr(true);
	Logger::setUseColors(false);
	//language=regexp
	check_msg(capture_err("Test"), R"(^\d\d:\d\d:\d\d.\d*[+\-]\d* - E - LoggerTests.cpp:\d*: Test\n$)");
}
