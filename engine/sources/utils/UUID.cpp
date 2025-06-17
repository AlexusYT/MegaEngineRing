//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 08.01.24.
//

#include <KwasarEngine/utils/UUID.h>
#include <regex>

#include "KwasarEngine/utils/ReportMessage.h"

namespace ke {
std::shared_ptr<UUID> UUID::parse(const std::string &pString, ReportMessagePtr &pError) {
	//language=regexp
	static std::regex regex(
		"([0-9a-fA-F]{8})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{8})([0-9a-fA-F]{4})");
	std::smatch matchResults;
	if (!std::regex_match(pString, matchResults, regex)) {
		if (!pError) pError = ReportMessage::create();
		pError->setTitle("Failed to parse the given string");
		pError->setMessage("The string doesn't match the regex");
		pError->addInfoLine("String to parse: {}", pString);
		return nullptr;
	}
	try {
		auto uuid = create();

		uuid->timeLow = static_cast<uint32_t>(std::stoul(matchResults[1].str(), nullptr, 16));
		uuid->timeMid = static_cast<uint16_t>(std::stoul(matchResults[2].str(), nullptr, 16));
		uuid->timeHiVersion = static_cast<uint16_t>(std::stoul(matchResults[3].str(), nullptr, 16));
		uuid->varClockSeq = static_cast<uint16_t>(std::stoul(matchResults[4].str(), nullptr, 16));
		uuid->node1 = static_cast<uint32_t>(std::stoul(matchResults[5].str(), nullptr, 16));
		uuid->node2 = static_cast<uint16_t>(std::stoul(matchResults[6].str(), nullptr, 16));
		return uuid;
	}
	catch (...) {
		if (!pError) pError = ReportMessage::create();
		pError->setTitle("Failed to parse the given string");
		pError->setMessage("Exception occurred while parsing the values");
		pError->addInfoLine("String to parse: {}", pString);
		return nullptr;
	}
}
} // namespace ke
