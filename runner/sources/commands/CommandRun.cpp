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
// Created by alexus on 11.03.24.
//

#include "CommandRun.h"

#include <dlfcn.h>

#include "Client.h"
#include "option/CommandOption.h"

namespace mer::runner {
CommandRun::CommandRun(Client* pClient) : client(pClient) {
	addCommand<CommandOption>("lib", 'l', "Lib to run method from")
		->setArg(CommandOption::REQUIRED_ARGUMENT)
		->setOnOptionParsedSlot([this](const std::string &pArg) {
			if (auto libSelf = client->getLib(pArg); !libSelf) {
				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to get library with the specified name");
				msg->setMessage("No library with such name loaded");
				msg->addInfoLine("Library name: {}", pArg);
				return msg;
			} else
				lib = libSelf;
			return sdk::utils::ReportMessagePtr{};
		});
	addCommand<CommandOption>("name", 'n', "Mangled name of the symbol to run")
		->setArg(CommandOption::REQUIRED_ARGUMENT)
		->setOnOptionParsedSlot([this](const std::string &pArg) {
			if (auto methodSelf = dlsym(lib, pArg.c_str())) {
				auto msg = sdk::utils::ReportMessage::create();
				msg->setTitle("Failed to get method with the specified name");
				msg->setMessage("No symbol with such name");
				msg->addInfoLine("Symbol name: {}", pArg);
				return msg;
			} else
				method = methodSelf;
			return sdk::utils::ReportMessagePtr{};
		});
}

void CommandRun::onParsingStarted() {
	lib = nullptr;
	method = nullptr;
}

void CommandRun::onParsingFinished() {
	/*void (*run)() = reinterpret_cast<void (*)()>(method);
	run();*/
}
} // namespace mer::runner