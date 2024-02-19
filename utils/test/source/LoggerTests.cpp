//
// Created by alexus on 19.02.24.
//
#include <gtest/gtest.h>
#include <regex>

#include "EngineUtils/utils/Logger.h"
using namespace n::engine::utils;

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