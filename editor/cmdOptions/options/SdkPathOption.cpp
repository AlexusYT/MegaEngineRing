//
// Created by alexus on 22.02.24.
//

#include "SdkPathOption.h"

#include "Globals.h"

namespace mer::editor::utils {
SdkPathOption::SdkPathOption(Glib::OptionGroup* const pOptionGroup)
	: Option(pOptionGroup, "sdk-path", "Path to sdk directory. Default: " + Globals::getSdkPath().string()) {}

bool SdkPathOption::onOptionParsed(const Glib::ustring & /*pUstring*/, const Glib::ustring &pValue, bool /*pCond*/) {
	using namespace sdk::utils;
	const std::filesystem::path sdkPath = std::filesystem::absolute(pValue.data());
	if (std::error_code err; !exists(sdkPath, err)) {
		const auto message = ReportMessage::create();
		message->setTitle("Failed to set sdk path");
		if (err) {
			message->setMessage("Error occured while checking path existance");
			message->addInfoLine("Error message: {}", err.message());
		} else {
			message->setMessage("No such directory");
		}
		message->addInfoLine("Path: {}", sdkPath.string());
		Logger::error(message);
		return false;
	}
	Globals::setSdkPath(sdkPath);

	return true;
}
} // namespace mer::editor::utils