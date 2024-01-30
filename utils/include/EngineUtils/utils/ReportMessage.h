//
// Created by alexus on 29.12.23.
//

#ifndef REPORTMESSAGE_H
#define REPORTMESSAGE_H
#include <stacktrace>
#include <vector>

#include "ReportMessageFwd.h"
#include "Utils.h"

namespace n::engine::utils {
class ReportMessage {

	std::string title;
	std::string message;
	std::vector<std::string> infoLines{};
	std::stacktrace stacktrace{};
	std::exception_ptr exceptionPtr;


	explicit ReportMessage(std::stacktrace pStacktrace);


public:
	static ReportMessageUPtr create(const std::stacktrace &pStacktrace = std::stacktrace::current()) {
		return ReportMessageUPtr(new (std::nothrow) ReportMessage(pStacktrace));
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

	[[nodiscard]] std::string getReport() const {
		std::stringstream ss;
		ss << (title.empty() ? "Untitled report" : title) << "\n";
		ss << "Message: " << message << "\n";

		if (exceptionPtr) {
			try {
				std::rethrow_exception(exceptionPtr);
			} catch (std::exception &e) {
				ss << "Exception info:\n";
				ss << "\tClass: " << Utils::getTypeName(&e) << "\n";
				ss << "\tMessage: " << e.what() << "\n";
			}
		}
		ss << "Additional info:\n";
		for (const auto &infoLine: infoLines) { ss << "\t" << infoLine << "\n"; }
		ss << "Stacktrace: \n" << stacktrace;
		return ss.str();
	}
};
} // namespace n::engine::utils


#endif //REPORTMESSAGE_H
