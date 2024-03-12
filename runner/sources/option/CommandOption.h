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

#ifndef COMMANDOPTION_H
#define COMMANDOPTION_H
#include <sigc++/functors/slot.h>
#include <string>

#include "EngineUtils/utils/ReportMessageFwd.h"
class CommandParser;
// ReSharper disable once CppInconsistentNaming
struct option;

class CommandOption {
	friend CommandParser;

public:
	enum CommandArg { NO_ARGUMENT, REQUIRED_ARGUMENT, OPTIONAL_ARGUMENT };

private:
	std::string longName;
	char shortName;
	std::string description;
	CommandArg arg{};
	CommandParser* parser{};
	using OnOptionParsedSlot = sigc::slot<mer::sdk::utils::ReportMessagePtr(const std::string &pArg)>;
	OnOptionParsedSlot onOptionParsedSlot;

public:
	CommandOption() = default;

	CommandOption(const std::string &pLongName, const char pShortName, const std::string &pDescription,
				  const OnOptionParsedSlot &pSlot = {})
		: longName(pLongName), shortName(pShortName), description(pDescription), onOptionParsedSlot(pSlot) {}

	virtual ~CommandOption() = default;

	[[nodiscard]] option getOption() const;

	[[nodiscard]] const std::string &getDescription() const { return description; }

	CommandOption* setDescription(const std::string &pDescription) {
		description = pDescription;
		return this;
	}

	[[nodiscard]] const std::string &getLongName() const { return longName; }

	CommandOption* setLongName(const std::string &pLongName) {
		longName = pLongName;
		return this;
	}

	[[nodiscard]] char getShortName() const { return shortName; }

	CommandOption* setShortName(const char pShortName) {
		shortName = pShortName;
		return this;
	}

	[[nodiscard]] CommandArg getArg() const { return arg; }

	CommandOption* setArg(const CommandArg pArg) {
		arg = pArg;
		return this;
	}

	[[nodiscard]] OnOptionParsedSlot &getOnOptionParsedSlot() { return onOptionParsedSlot; }

	CommandOption* setOnOptionParsedSlot(const OnOptionParsedSlot &pOnOptionParsedSlot) {
		onOptionParsedSlot = pOnOptionParsedSlot;
		return this;
	}

protected:
	[[nodiscard]] CommandParser* getParser() const { return parser; }

	void setParser(CommandParser* const pParser) { parser = pParser; }

private:
	virtual mer::sdk::utils::ReportMessagePtr onOptionParsed(const std::string &pArg);
};


#endif //COMMANDOPTION_H
