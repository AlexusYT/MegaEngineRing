//  KwasarEngine is an SDK that can help you speed up game development.
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
#include <nlohmann/json_fwd.hpp>

#include "ReportMessageFwd.h"
#include "Utils.h"

namespace ke {
class ReportMessage {
	class StacktraceEntry {
		std::string description;
		uint_least32_t line{};
		std::string file;

	public:
		StacktraceEntry() = default;

		StacktraceEntry(const std::string &pDescription, uint_least32_t pLine, const std::string &pFile)
			: description(pDescription),
			  line(pLine),
			  file(pFile) {}

		[[nodiscard]] const std::string &getDescription() const { return description; }

		[[nodiscard]] uint_least32_t getLine() const { return line; }

		[[nodiscard]] const std::string &getFile() const { return file; }

		void serialize(nlohmann::json &pJson) const;

		void deserialize(const nlohmann::json &pJson);

		bool isEmpty() const { return description.empty() && file.empty() && line == 0; }

		friend void to_json(nlohmann::json &pJson, const StacktraceEntry &pMsg) { pMsg.serialize(pJson); }

		friend void from_json(const nlohmann::json &pJson, StacktraceEntry &pMsg) { pMsg.deserialize(pJson); }
	};

	std::string title;
	std::string message;
	std::vector<std::string> infoLines{};
	std::vector<StacktraceEntry> stacktrace{};
	std::exception_ptr exceptionPtr;


	explicit ReportMessage(const std::stacktrace &pStacktrace);

public:
	static ReportMessagePtr create(const std::stacktrace &pStacktrace = std::stacktrace::current()) {
		return ReportMessagePtr(new(std::nothrow) ReportMessage(pStacktrace));
	}

	[[nodiscard]] const std::string &getTitle() const { return title; }

	void setTitle(const std::string &pTitle) { title = pTitle; }

	[[nodiscard]] const std::string &getMessage() const { return message; }

	void setMessage(const std::string &pMessage) { message = pMessage; }

	template <typename... Args>
	void addInfoLine(const std::string_view pFmt, Args &&... pArgs) {
		addInfoLine(std::vformat(pFmt, std::make_format_args(pArgs...)));
	}

	void addInfoLine(const std::string &pInfoLine) { infoLines.emplace_back(pInfoLine); }

	void setStacktrace(const std::stacktrace &pStacktrace = std::stacktrace::current());

	void setExceptionPtr(const std::exception_ptr &pExceptionPtr) { exceptionPtr = pExceptionPtr; }

	[[nodiscard]] std::string getReport(const bool pShowStacktrace = true) const {
		std::stringstream ss;
		ss << (title.empty() ? "Untitled report" : title) << "\n";
		ss << "Message: " << message << "\n";

		if (exceptionPtr) {
			try { std::rethrow_exception(exceptionPtr); }
			catch (std::exception &e) {
				ss << "Exception info:\n";
				ss << "\tClass: " << Utils::getTypeName(&e) << "\n";
				ss << "\tMessage: " << e.what() << "\n";
			}
		}
		ss << "Additional info:\n";
		for (const auto &infoLine: infoLines) { ss << "\t" << infoLine << "\n"; }
		if (pShowStacktrace) {
			ss << "Stacktrace: \n";
			int skipped = 0;
			for (uint32_t i = 0; auto &entry: stacktrace) {
				if (!entry.isEmpty()) {
					if (skipped > 0) ss << "      Skipped " << skipped << " empty frames" << "\n";
					ss << std::setw(4) << i << "# " << entry.getDescription() << " at " << entry.getFile() <<
						":" << entry.getLine() << "\n";
					skipped = 0;
				} else
					skipped++;

				i++;
			}
		}
		return ss.str();
	}

	void serialize(nlohmann::json &pJson) const;

	void deserialize(const nlohmann::json &pJson);

	friend void to_json(nlohmann::json &pJson, const ReportMessagePtr &pMsg) { pMsg->serialize(pJson); }

	friend void from_json(const nlohmann::json &pJson, ReportMessagePtr &pMsg) {
		pMsg = create();
		pMsg->deserialize(pJson);
	}
};
} // namespace ke


#endif //REPORTMESSAGE_H
