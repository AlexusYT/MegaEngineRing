//
// Created by alexus on 31.12.23.
//

#include <EngineUtils/utils/Utils.h>
#include <cxxabi.h>

std::string Utils::demangle(const std::string &pMangledName) {
	return abi::__cxa_demangle(pMangledName.c_str(), nullptr, nullptr, nullptr);
}