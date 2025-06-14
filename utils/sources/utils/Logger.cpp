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
// Created by alexus on 29.12.23.
//

#include <EngineUtils/utils/Logger.h>
#include <cstring>
#include <chrono>
#include <iostream>
#include <stacktrace>

#include "ConsoleColors.h"

namespace mer::sdk {
ReportMessagePtr Logger::openLog(const std::filesystem::path &pLogPath) {
	if (pLogPath.empty()) {
		auto report = ReportMessage::create();
		report->setTitle("Failed to open new log.");
		report->setMessage("Specified path is empty");
		return report;
	}
	if (logFile.is_open()) {
		auto report = ReportMessage::create();
		report->setTitle("Failed to open new log.");
		report->setMessage("Log already opened. Close it first");
		report->addInfoLine("Log opened at {}", logPath.string());
		report->addInfoLine("Trying to open at {}", pLogPath.string());
		return report;
	}
	logPath = pLogPath;
	try {
		logFile.exceptions(std::_S_badbit | std::_S_eofbit | std::_S_failbit);
		logFile.open(pLogPath);
	}
	catch (...) {
		auto report = ReportMessage::create();
		report->setTitle("Failed to open new log.");
		report->setMessage("Exception thrown");
		report->addInfoLine("Trying to open log at {}", pLogPath.string());
		return report;
	}


	return nullptr;
}

void Logger::print(const LoggerMsgType pType, const std::string &pMessage) {
	try {
		using namespace std::chrono;

		auto local = zoned_time{current_zone(), system_clock::now()};

		const auto stacktrace = std::stacktrace::current(2, 1);
		const auto frame = stacktrace.at(0);

		const std::filesystem::path file = frame.source_file();


		std::string source = std::format("{}:{}", file.filename().string(), frame.source_line());
		std::string time = std::format("{0:%T}{0:%z}", local);
		std::string type;
		std::ostream &(*color)(std::ostream &) = ConsoleColors::DARK_WHITE;
		switch (pType) {
			case LoggerMsgType::INFO:
				type = "I";
				color = ConsoleColors::DARK_GREEN;
				break;
			case LoggerMsgType::WARN:
				type = "W";
				color = ConsoleColors::LIGHT_YELLOW;
				break;
			case LoggerMsgType::ERROR:
				type = "E";
				color = ConsoleColors::DARK_RED;
				break;
			case LoggerMsgType::OUT:
				type = "O";
				color = ConsoleColors::DARK_BLUE;
				break;
			default: type = "UNKNOWN";
		}


		const std::string result = std::format("{} - {} - {}: {}", time, type, source, pMessage);
		if (logFile.is_open()) {
			logFile << result << "\n";
			logFile.flush();
		}
		if (useColors)
			(useCErr && pType == LoggerMsgType::ERROR ? std::cerr : std::cout)
				<< color << result << ConsoleColors::CLEAR << '\n';
		else
			(useCErr && pType == LoggerMsgType::ERROR ? std::cerr : std::cout) << result << '\n';
	}
	catch (...) { processException(pMessage); }
	std::cout.flush();
}

void Logger::processException(const std::string &pFailedMessage) {
	auto msg = ReportMessage::create();
	msg->setTitle("Failed to print the logger message");
	msg->setMessage("Exception occurred");
	msg->addInfoLine("Failed message: " + pFailedMessage);
	std::string s = msg->getReport();
	std::cout << s << "\n";
	if (logFile.is_open()) {
		logFile << s << "\n";
		logFile.flush();
	}
}
} // namespace mer::sdk
