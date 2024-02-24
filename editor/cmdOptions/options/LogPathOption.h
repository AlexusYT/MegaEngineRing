//
// Created by alexus on 31.12.23.
//

#ifndef LOGPATHOPTION_H
#define LOGPATHOPTION_H


#include "Globals.h"
#include "Option.h"

namespace mer::editor::utils {

class LogPathOption final : public Option {

public:
	explicit LogPathOption(Glib::OptionGroup* pOptionGroup)
		: Option(pOptionGroup, "log_path", "Path to logs directory. Default: " + Globals::getLogPath().string()) {

		set_flags(Flags::FILENAME);
	}

	bool onOptionParsed(const Glib::ustring & /*pOptionName*/, const Glib::ustring &pValue,
						bool /*pHasValue*/) override {
		using namespace sdk::utils;
		std::filesystem::path logPath = std::filesystem::absolute(pValue.data());
		if (std::error_code err; !exists(logPath, err)) {
			const auto message = ReportMessage::create();
			message->setTitle("Failed to set logs directory");
			if (err) {
				message->setMessage("Error occured while checking path existance");
				message->addInfoLine("Error message: {}", err.message());
			} else {
				message->setMessage("No such directory");
			}
			message->addInfoLine("Path: {}", logPath.string());
			Logger::error(message);
			return false;
		}
		Globals::setLogPath(logPath);

		return true;
	}
};

} // namespace mer::editor::utils

#endif //LOGPATHOPTION_H
