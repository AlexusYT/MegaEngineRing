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

#ifndef COMMAND_H
#define COMMAND_H
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

#include "EngineUtils/utils/ReportMessageFwd.h"

namespace mer::runner {
class Command {
	std::string command;
	std::vector<std::string> args;

public:
	mer::sdk::utils::ReportMessagePtr execute();

	[[nodiscard]] const std::string &getCommand() const { return command; }

	void setCommand(const std::string &pCommand) { command = pCommand; }

	[[nodiscard]] const std::vector<std::string> &getArgs() const { return args; }

	void setArgs(const std::vector<std::string> &pArgs) { args = pArgs; }

	friend void to_json(nlohmann::json &pNlohmannJsonJ, const Command &pNlohmannJsonT);

	friend void from_json(const nlohmann::json &pNlohmannJsonJ, Command &pNlohmannJsonT);
};
} // namespace mer::runner


#endif //COMMAND_H
