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

#ifndef REPORTMESSAGE_H
#define REPORTMESSAGE_H
#include <stacktrace>
#include <vector>

#include "ReportMessageFwd.h"

namespace mer::sdk {
class ReportMessage {

	std::string title;
	std::string message;
	std::vector<std::string> infoLines{};
	std::stacktrace stacktrace{};
	std::exception_ptr exceptionPtr;


	explicit ReportMessage(std::stacktrace pStacktrace);


public:
	static ReportMessagePtr create(const std::stacktrace &pStacktrace = std::stacktrace::current()) {
		return ReportMessagePtr(new (std::nothrow) ReportMessage(pStacktrace));
	}

	[[nodiscard]] const std::string &getTitle() const { return title; }

	void setTitle(const std::string &pTitle) { title = pTitle; }

	[[nodiscard]] const std::string &getMessage() const { return message; }

	void setMessage(const std::string &pMessage) { message = pMessage; }

	template<typename... Args>
	void addInfoLine(const std::string_view pFmt, Args &&... pArgs) {
		addInfoLine(std::vformat(pFmt, std::make_format_args(pArgs...)));
	}

	void addInfoLine(const std::string &pInfoLine) { infoLines.emplace_back(pInfoLine); }

	void setStacktrace(const std::stacktrace &pStacktrace = std::stacktrace::current()) { stacktrace = pStacktrace; }

	void setExceptionPtr(const std::exception_ptr &pExceptionPtr) { exceptionPtr = pExceptionPtr; }

	[[nodiscard]] std::string getReport(bool pShowStacktrace = true) const;
};
} // namespace mer::sdk


#endif //REPORTMESSAGE_H
