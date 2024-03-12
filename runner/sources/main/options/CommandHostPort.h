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
// Created by alexus on 08.03.24.
//

#ifndef COMMANDHOSTPORT_H
#define COMMANDHOSTPORT_H
#include "option/CommandOption.h"

class CommandHostPort : public CommandOption {
public:
	CommandHostPort()
		: CommandOption("port", 'p',
						std::format("Port to run tcp connection [1000-65535]. Default is {}", Globals::hostPort)) {
		setArg(REQUIRED_ARGUMENT);
	}

	mer::sdk::utils::ReportMessagePtr onOptionParsed(const std::string &pArg) override {
		if (!pArg.empty()) Globals::hostPort = static_cast<uint16_t>(std::stoi(pArg, nullptr, 10));
		return nullptr;
	}
};


#endif //COMMANDHOSTPORT_H
