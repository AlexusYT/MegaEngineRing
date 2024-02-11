//
// Created by alexus on 31.12.23.
//

#include <EngineUtils/utils/Utils.h>
#include <cxxabi.h>
#include <regex>

void replace_name(std::string &pIn, const std::string &pWhat, const std::string &pReplaceWith) {
	std::regex regex(pWhat);
	pIn = std::regex_replace(pIn, regex, pReplaceWith);
	/*if (auto iter = pIn.find(pWhat); iter != std::string::npos) {
		//
		pIn.replace(iter, pWhat.size(), pReplaceWith);
	}*/
}

std::string Utils::demangle(const std::string &pMangledName) {
	if (const auto str = abi::__cxa_demangle(pMangledName.c_str(), nullptr, nullptr, nullptr)) {
		try {
			std::string name = str;

			replace_name(name, "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
						 "std::string");
			return name;
		} catch (...) { return str; }
	}
	return pMangledName;
}

std::string Utils::parseDlError(const std::string &pMsg) {

	if (auto iter = pMsg.find("undefined symbol: "); iter != std::string::npos) {
		return "undefined symbol: " + demangle(pMsg.substr(iter + 18));
	}
	return pMsg;
}