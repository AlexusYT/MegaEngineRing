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
// Created by alexus on 23.02.24.
//

#include "ResPathOption.h"

#include "Globals.h"

namespace mer::editor::utils {
ResPathOption::ResPathOption(Glib::OptionGroup* pOptionGroup)
	: Option(pOptionGroup, "res-path",
			 "Path to resources directory. Default: " + Globals::getResourcesPath().parent_path().string()) {}

bool ResPathOption::onOptionParsed(const Glib::ustring & /*pUstring*/, const Glib::ustring &pValue, bool /*pCond*/) {
	using namespace sdk::utils;
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
} // namespace mer::editor::utils