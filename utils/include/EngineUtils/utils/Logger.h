//
// Created by alexus on 29.12.23.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <format>
#include <fstream>

#include "ReportMessage.h"

namespace mer::sdk::utils {
enum class LoggerMsgType : uint8_t { INFO, WARN, ERROR, OUT };

class Logger {
	inline static bool useColors = true;
	inline static bool useCErr = false;
	inline static std::filesystem::path logPath;
	inline static std::ofstream logFile;

public:
	static ReportMessagePtr openLog(const std::filesystem::path &pLogPath);

	static void info(const std::string &pStr) { print(LoggerMsgType::INFO, pStr); }

	template<typename... Args>
	static void info(const std::string_view pFmt, Args &&... pArgs) {
		try {
			print(LoggerMsgType::INFO, std::vformat(pFmt, std::make_format_args(pArgs...)));
		} catch (...) { processException({pFmt.begin(), pFmt.end()}); }
	}

	static void warn(const std::string &pStr) { print(LoggerMsgType::WARN, pStr); }

	template<typename... Args>
	static void warn(const std::string_view pFmt, Args &&... pArgs) {
		try {
			print(LoggerMsgType::WARN, std::vformat(pFmt, std::make_format_args(pArgs...)));
		} catch (...) { processException({pFmt.begin(), pFmt.end()}); }
	}

	static void error(const ReportMessagePtr &pMessage) { print(LoggerMsgType::ERROR, pMessage->getReport()); }

	static void error(const std::string &pStr) { print(LoggerMsgType::ERROR, pStr); }

	template<typename... Args>
	static void error(const std::string_view pFmt, Args &&... pArgs) {
		try {
			print(LoggerMsgType::ERROR, std::vformat(pFmt, std::make_format_args(pArgs...)));
		} catch (...) { processException({pFmt.begin(), pFmt.end()}); }
	}

	static void out(const std::string &pStr) { print(LoggerMsgType::OUT, pStr); }

	template<typename... Args>
	static void out(const std::string_view pFmt, Args &&... pArgs) {
		try {
			print(LoggerMsgType::OUT, std::vformat(pFmt, std::make_format_args(pArgs...)));
		} catch (...) { processException({pFmt.begin(), pFmt.end()}); }
	}

	template<typename... Args>
	static void print(const LoggerMsgType pType, const std::string_view pFmt, Args &&... pArgs) {
		try {
			print(pType, std::vformat(pFmt, std::make_format_args(pArgs...)));
		} catch (...) { processException({pFmt.begin(), pFmt.end()}); }
	}

	static void print(LoggerMsgType pType, const std::string &pMessage);

	[[nodiscard]] static bool isUseColors() { return useColors; }

	static void setUseColors(const bool pUseColors) { useColors = pUseColors; }

	[[nodiscard]] static bool isUseCErr() { return useCErr; }

	static void setUseCErr(const bool pUseCErr) { useCErr = pUseCErr; }

	[[nodiscard]] static const std::filesystem::path &getLogPath() { return logPath; }

	static void setLogPath(const std::filesystem::path &pLogPath) { logPath = pLogPath; }

private:
	static void processException(const std::string &pFailedMessage);
};
} // namespace mer::sdk::utils


#endif //LOGGER_H
