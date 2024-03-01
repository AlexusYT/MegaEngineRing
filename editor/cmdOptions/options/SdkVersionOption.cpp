// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 22.02.24.
//

#include "SdkVersionOption.h"

#include <regex>

#include "Globals.h"

namespace mer::editor::utils {
SdkVersionOption::SdkVersionOption(Glib::OptionGroup* pOptionGroup)
	: Option(pOptionGroup, "sdk-version", "Sdk version. Default: randomly selected") {}

bool SdkVersionOption::onOptionParsed(const Glib::ustring & /*pUstring*/, const Glib::ustring &pValue, bool /*pCond*/) {
	//language=regexp
	if (auto reg = R"(^\d+\.\d+\.\d+$)"; !std::regex_match(std::string(pValue), std::regex(reg))) {

		const auto message = sdk::utils::ReportMessage::create();
		message->setTitle("Failed to set sdk version");
		message->setMessage("Specified version is invalid");
		message->addInfoLine("Version must match regex: {}", reg);
		sdk::utils::Logger::error(message);
		return false;
	}
	if (pValue.empty()) {}
	Globals::setSdkVersion(pValue);
	return true;
}
} // namespace mer::editor::utils