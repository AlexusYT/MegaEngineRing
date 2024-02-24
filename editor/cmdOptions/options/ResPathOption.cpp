//
// Created by alexus on 23.02.24.
//

#include "ResPathOption.h"

#include "Globals.h"

namespace UTILS_CORE {
ResPathOption::ResPathOption(Glib::OptionGroup* pOptionGroup)
	: Option(pOptionGroup, "res-path",
			 "Path to resources directory. Default: " + Globals::getResourcesPath().parent_path().string()) {}

bool ResPathOption::onOptionParsed(const Glib::ustring & /*pUstring*/, const Glib::ustring &pValue, bool /*pCond*/) {
	using namespace engine::utils;
	const std::filesystem::path resPath = std::filesystem::absolute(pValue.data()) / "Resources";
	if (std::error_code err; !exists(resPath, err)) {
		const auto message = ReportMessage::create();
		message->setTitle("Failed to set resources path");
		if (err) {
			message->setMessage("Error occured while checking path existance");
			message->addInfoLine("Error message: {}", err.message());
		} else {
			message->setMessage("There is no resources at the given path");
		}
		message->addInfoLine("Path: {}", resPath.string());
		Logger::error(message);
		return false;
	}
	Globals::setResourcesPath(resPath);
	return true;
}
} // namespace UTILS_CORE