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
// Created by alexus on 04.03.24.
//

#include "Command.h"

#include <dlfcn.h>
#include <nlohmann/json.hpp>

#include "../Globals.h"
#include "EngineUtils/utils/ReportMessage.h"

namespace mer::runner {
mer::sdk::utils::ReportMessagePtr Command::execute() {

	if (command == "load") {

		if (args.size() > 1 || args.size() < 1) {
			auto msg = mer::sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to execute the command");
			if (args.size() < 1) msg->setMessage("Too few arguments");
			else
				msg->setMessage("Too many arguments");
			msg->addInfoLine("Expected: 1");
			msg->addInfoLine("Actual: {}", args.size());
			msg->addInfoLine("Command: {}", command);
			return msg;
		}
		std::string libPath = args[0];
		Globals::editorLib = dlopen((libPath).c_str(), RTLD_NOW | RTLD_GLOBAL);
		if (!Globals::editorLib) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to open editor library");
			msg->setMessage("The function dlopen() returned error");
			msg->addInfoLine("Path: {}", libPath);
			msg->addInfoLine("Error: {}", Utils::parseDlError(dlerror()));
			return msg;
		}
	} else {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to execute the command");
		msg->setMessage("Unknown command");
		msg->addInfoLine("Specified command: {}", command);
		return msg;
	}

	return nullptr;
}

void to_json(nlohmann::json &pNlohmannJsonJ, const Command &pNlohmannJsonT) {
	pNlohmannJsonJ["command"] = pNlohmannJsonT.command;
	pNlohmannJsonJ["args"] = pNlohmannJsonT.args;
}

void from_json(const nlohmann::json &pNlohmannJsonJ, Command &pNlohmannJsonT) {
	pNlohmannJsonJ.at("command").get_to(pNlohmannJsonT.command);

	std::vector<std::string> argsList;
	pNlohmannJsonJ.at("args").get_to(argsList);
	pNlohmannJsonT.args.emplace_back(pNlohmannJsonT.command);
	for (const auto &arg: argsList) {
		if (!arg.empty()) pNlohmannJsonT.args.emplace_back(arg);
	}
}
} // namespace mer::runner