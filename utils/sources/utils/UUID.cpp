//
// Created by alexus on 08.01.24.
//

#include <EngineUtils/utils/UUID.h>
#include <regex>

std::shared_ptr<UUID> UUID::parse(const std::string &pString, mer::sdk::utils::ReportMessagePtr &pError) {
	//language=regexp
	static std::regex regex(
		"([0-9a-fA-F]{8})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{4})-([0-9a-fA-F]{8})([0-9a-fA-F]{4})");
	std::smatch matchResults;
	if (!std::regex_match(pString, matchResults, regex)) {
		if (!pError) pError = mer::sdk::utils::ReportMessage::create();
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
	} catch (...) {
		if (!pError) pError = mer::sdk::utils::ReportMessage::create();
		pError->setTitle("Failed to parse the given string");
		pError->setMessage("Exception occurred while parsing the values");
		pError->addInfoLine("String to parse: {}", pString);
		return nullptr;
	}
}