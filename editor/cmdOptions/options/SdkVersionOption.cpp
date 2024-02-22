//
// Created by alexus on 22.02.24.
//

#include "SdkVersionOption.h"

#include <regex>

#include "Globals.h"

namespace UTILS_CORE {
SdkVersionOption::SdkVersionOption(Glib::OptionGroup* pOptionGroup)
	: Option(pOptionGroup, "sdk-version", "Sdk version. Default: randomly selected") {}

bool SdkVersionOption::onOptionParsed(const Glib::ustring & /*pUstring*/, const Glib::ustring &pValue, bool /*pCond*/) {
	//language=regexp
	if (auto reg = R"(^\d+\.\d+\.\d+$)"; !std::regex_match(std::string(pValue), std::regex(reg))) {

		const auto message = engine::utils::ReportMessage::create();
		message->setTitle("Failed to set sdk version");
		message->setMessage("Specified version is invalid");
		message->addInfoLine("Version must match regex: {}", reg);
		engine::utils::Logger::error(message);
		return false;
	}
	if (pValue.empty()) {}
	Globals::setSdkVersion(pValue);
	return true;
}
} // namespace UTILS_CORE