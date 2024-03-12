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
// Created by alexus on 01.03.24.
//


#include <asio.hpp>
#include <csignal>
#include <getopt.h>
#include <nlohmann/json_fwd.hpp>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>

#include "Client.h"
#include "EngineUtils/utils/Logger.h"
#include "Globals.h"
#include "RequestSchema.h"
#include "main/options/CommandHelp.h"
#include "main/options/CommandHostAddress.h"
#include "main/options/CommandHostPort.h"
#include "option/CommandOption.h"
#include "option/CommandParser.h"

using namespace mer::sdk::utils;
using namespace valijson;

ReportMessagePtr build_schema(const nlohmann::json &pSchema, const std::shared_ptr<Schema> &pSchemaOut) {
	using namespace valijson::adapters;


	// Parse the json schema into an internal schema format
	SchemaParser parser(SchemaParser::kDraft7);

	const NlohmannJsonAdapter schemaDocumentAdapter(pSchema);
	try {
		parser.populateSchema(schemaDocumentAdapter, *pSchemaOut);
	} catch (...) {
		auto msg = ReportMessage::create();
		msg->setMessage("Failed to parse schema");
		return msg;
	}
	return nullptr;
}

std::vector<std::shared_ptr<mer::runner::Client>> clients;

void handler(int pSig) {
	nlohmann::json response;
	response["type"] = "response";
	response["nonce"] = -1;
	response["data"]["status"] = "error";
	response["data"]["error"] = "internal_server_error";
	auto msg = ReportMessage::create();
	msg->setTitle("Runner crashed");
	msg->setMessage("Signal handled");

	std::string signalName;
	switch (pSig) {
		case SIGTERM: signalName = "SIGTERM"; break;
		case SIGABRT: signalName = "SIGABRT"; break;
		case SIGSEGV: signalName = "SIGSEGV"; break;
		default: signalName = "unknown";
	}
	msg->addInfoLine("Signal: {}", signalName);
	response["data"]["report"] = msg;

	int i = 0;
	for (const auto &client: clients) {
		i++;
		client->sendJson(response);
	}
	exit(1);
}

void usage() {
	std::cout << "usage: " << std::endl;
	std::cout << std::setw(4) << "usage: " << std::endl;
}

//int main(int pArgc, char* pArgv[]) {
int main(int pArgc, char* const* pArgv) {

	CommandParser parser;
	parser.addCommand(std::make_shared<CommandHelp>());
	parser.addCommand(std::make_shared<CommandHostPort>());
	parser.addCommand(std::make_shared<CommandHostAddress>());

	if (auto msg = parser.parse(pArgc, pArgv); msg && msg->getMessage().empty()) {
		return 0;
	} else if (msg) {
		Logger::error(msg);
		return -1;
	}

	signal(SIGTERM, handler);
	signal(SIGABRT, handler);
	signal(SIGSEGV, handler);
	if (auto msg = Logger::openLog("log")) { Logger::error(msg); }
	Globals::schema = std::make_shared<Schema>();
	if (const auto msg = build_schema(nlohmann::json::parse(requestSchemaJson), Globals::schema)) {
		Logger::error(msg);
		return -1;
	}


	Logger::info("Starting runner at {}:{}", Globals::hostAddress, Globals::hostPort);
	try {
		asio::io_context io_context;
		const asio::ip::address IP(asio::ip::make_address(Globals::hostAddress));
		asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(IP, Globals::hostPort));
		while (true) {
			Logger::info("Accepting connection");
			asio::ip::tcp::socket socket(io_context);
			acceptor.accept(socket);
			Logger::info("Connection accepted");

			clients.emplace_back(std::make_shared<mer::runner::Client>(socket))->run();
		}

	} catch (std::exception &e) { std::cerr << e.what() << std::endl; }

	return 0;
}