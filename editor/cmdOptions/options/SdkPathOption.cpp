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