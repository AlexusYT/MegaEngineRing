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

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include <memory>
#include <unordered_map>

#include "EngineUtils/utils/ReportMessage.h"


class CommandOption;

// ReSharper disable once CppInconsistentNaming
struct option;

class CommandParser {
	std::unordered_map<int, std::shared_ptr<CommandOption>> commands;

public:
	CommandParser();
	virtual ~CommandParser() = default;

	template<typename ClassT, typename... ArgsT>
	ClassT* addCommand(ArgsT... pArgs) {
		return dynamic_cast<ClassT*>(addCommand(std::make_shared<ClassT>(pArgs...)));
	}

	CommandOption* addCommand(const std::shared_ptr<CommandOption> &pCommand);

	mer::sdk::utils::ReportMessagePtr parse(int pArgc, char* const* pArgv);

	[[nodiscard]] const std::unordered_map<int, std::shared_ptr<CommandOption>> &getCommands() const {
		return commands;
	}

private:
	virtual void onParsingStarted() {}

	virtual void onParsingFinished() {}
};


#endif //COMMANDPARSER_H
