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

#ifndef CLIENT_H
#define CLIENT_H
#include <asio/ip/tcp.hpp>
#include <filesystem>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json_fwd.hpp>

#include "EngineUtils/utils/ReportMessageFwd.h"


class CommandParser;

namespace mer::sdk::utils {
class ReportMessage;
}

namespace mer::sdk::utils::network {
class ClientConnection;
}

namespace mer::runner {
class Client {
	asio::ip::tcp::socket socket;
	std::unordered_map<std::string, void*> loadedLibs;

	static std::unordered_map<std::string, std::shared_ptr<CommandParser>> commandParsers;

public:
	explicit Client(asio::ip::tcp::socket &pSocket);

	void run();

	void sendJson(const nlohmann::json &pJson);

	sdk::utils::ReportMessagePtr addLib(const std::filesystem::path &pLibPath);

	void* getLib(const std::string &pLibName) {
		if (const auto iter = loadedLibs.find(pLibName); iter == loadedLibs.end()) return nullptr;
		else
			return iter->second;
	}
};

} // namespace mer::runner

namespace mer::sdk::utils {
extern void to_json(nlohmann::json &pNlohmannJsonJ, const ReportMessagePtr &pNlohmannJsonT);
}

#endif //CLIENT_H
