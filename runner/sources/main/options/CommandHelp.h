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
// Created by alexus on 09.03.24.
//

#ifndef COMMANDHELP_H
#define COMMANDHELP_H
#include <iomanip>
#include <iostream>

#include "option/CommandOption.h"
#include "option/CommandParser.h"

class CommandHelp : public CommandOption {

public:
	CommandHelp() : CommandOption("help", 'h', "Help command") {}

private:
	mer::sdk::utils::ReportMessagePtr onOptionParsed(const std::string & /*pArg*/) override {
		std::cout << R"(Help:
MegaEngineRing <options>
Options:
)";
		for (const auto &[fst, option]: getParser()->getCommands()) {
			auto shortNameSelf = option->getShortName();
			std::cout << std::right;
			(shortNameSelf != 0 ? std::cout << std::setw(4) << "-" << shortNameSelf << ", " : std::cout << std::setw(9))
				<< "--" << std::setw(10) << std::left << option->getLongName() << option->getDescription();
			std::cout << std::endl;
		}
		return mer::sdk::utils::ReportMessage::create();
	}
};


#endif //COMMANDHELP_H
