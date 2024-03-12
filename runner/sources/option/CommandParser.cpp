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
// Created by alexus on 07.03.24.
//

#include "CommandParser.h"

#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "CommandOption.h"

CommandParser::CommandParser() = default;

CommandOption* CommandParser::addCommand(const std::shared_ptr<CommandOption> &pCommand) {
	pCommand->setParser(this);
	auto shortName = pCommand->getShortName();
	int id = 0;
	if (shortName > 0) id = shortName;
	else
		id = static_cast<int>(commands.size() + 1000);
	return commands.emplace(id, pCommand).first->second.get();
}

mer::sdk::utils::ReportMessagePtr CommandParser::parse(int pArgc, char* const* pArgv) {
	if (commands.empty()) return nullptr;
	std::vector<option> options;
	options.reserve(commands.size() + 1);
	std::string shorts;
	for (const auto &command: commands) {

		auto shortName = command.second->getShortName();
		if (shortName > 0) {
			shorts.push_back(shortName);
			if (command.second->getArg() != CommandOption::NO_ARGUMENT) { shorts.push_back(':'); }
		}
		auto option = command.second->getOption();
		option.val = command.first;
		options.emplace_back(option);
	}
	options.emplace_back();
	onParsingStarted();
	while (true) {
		int optIdx{};
		if (const int c = getopt_long(pArgc, pArgv, shorts.c_str(), options.data(), &optIdx); c > 0) {
			if (auto iter = commands.find(c); iter != commands.end())
				if (auto msg = iter->second->onOptionParsed(optarg ? optarg : "")) { return msg; }
			if (c == '?') {
				auto msg = mer::sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to execute command");
				msg->setMessage("Unrecognized option");
				msg->addInfoLine("Command: {}", pArgv[0]);
				msg->addInfoLine("Option: {}", pArgv[optIdx + 1]);

				return msg;
			}
		} else
			break;
	}
	onParsingFinished();
	return nullptr;
}