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
// Created by alexus on 31.12.23.
//

#ifndef LOGPATHOPTION_H
#define LOGPATHOPTION_H


#include "EngineUtils/utils/Logger.h"
#include "Globals.h"
#include "Option.h"

namespace mer::editor {

class LogPathOption final : public Option {

public:
	explicit LogPathOption(Glib::OptionGroup* pOptionGroup)
		: Option(pOptionGroup, "log_path", "Path to logs directory. Default: " + Globals::getLogPath().string()) {

		set_flags(Flags::FILENAME);
	}

	bool onOptionParsed(const Glib::ustring & /*pOptionName*/, const Glib::ustring &pValue,
						bool /*pHasValue*/) override {
		using namespace sdk;
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

} // namespace mer::editor

#endif //LOGPATHOPTION_H
