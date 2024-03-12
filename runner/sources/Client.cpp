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

#include "Client.h"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#define VALIJSON_USE_EXCEPTIONS 1 // NOLINT(modernize-macro-to-enum)
#include <asio.hpp>
#include <asio/read_until.hpp>
#include <asio/streambuf.hpp>
#include <asio/write.hpp>
#include <dlfcn.h>
#include <sigc++/connection.h>
#include <sigc++/functors/mem_fun.h>
#include <valijson/adapters/nlohmann_json_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "EngineUtils/utils/Logger.h"
#include "Globals.h"
#include "commands/CommandRun.h"
#include "option/CommandOption.h"
#include "option/CommandParser.h"
#include "requests/Command.h"

namespace mer::runner {
std::unordered_map<std::string, std::shared_ptr<CommandParser>> Client::commandParsers;

using namespace valijson;

sdk::utils::ReportMessagePtr validateJson(const std::shared_ptr<Schema> &pSchema, const nlohmann::json &pFileJson) {
	using namespace valijson::adapters;

	// Perform validation
	Validator validator(Validator::kStrongTypes);
	ValidationResults results;
	const NlohmannJsonAdapter targetDocumentAdapter(pFileJson);
	if (!validator.validate(*pSchema, targetDocumentAdapter, &results)) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setMessage("Validation failed");
		ValidationResults::Error validationError;
		unsigned int errorNum = 1;
		while (results.popError(validationError)) {

			std::string context;
			for (auto &itr: validationError.context) { context += itr; }

			msg->addInfoLine("Error #{}: ", errorNum);
			msg->addInfoLine(" Context: {}: ", context);
			msg->addInfoLine(" Description: {}: ", validationError.description);
			++errorNum;
		}
		return msg;
	}
	return nullptr;
}

Client::Client(asio::ip::tcp::socket &pSocket) : socket(std::move(pSocket)) {

	auto libParser = std::make_shared<CommandParser>();
	libParser->addCommand<CommandOption>("load", 'l', "Load lib")
		->setArg(CommandOption::REQUIRED_ARGUMENT)
		->setOnOptionParsedSlot([this](const std::string &pArg) { return addLib(pArg); });
	commandParsers.emplace("lib", libParser);
	auto runParser = std::make_shared<CommandRun>(this);
	commandParsers.emplace("run", runParser);
}

void Client::run() {

	while (true) {
		std::error_code error;
		asio::streambuf buff;
		auto bytes = asio::read_until(socket, buff, '}', error);
		if (error == asio::error::eof) {
			sdk::utils::Logger::warn("Connection closed cleanly by peer");
			break; // Connection closed cleanly by peer.
		}
		if (error) throw asio::system_error(error); // Some other error.
		nlohmann::json json;
		std::stringstream ss;
		ss << &buff;
		while (ss.get() != '{')
			;
		ss.seekg(-1, std::_S_cur);
		std::string s = ss.str();
		sdk::utils::Logger::info(s);
		try {
			ss >> json;
		} catch (...) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Invalid request");
			msg->setMessage("Failed to parse json");
			sdk::utils::Logger::error(msg);

			nlohmann::json response;
			response["type"] = "response";
			response["nonce"] = json["nonce"];
			response["data"]["status"] = "error";
			response["data"]["error"] = "invalid_request";
			response["data"]["report"] = msg;
			sendJson(response);
			continue;
		}
		sdk::utils::Logger::info("Read {} bytes", bytes);
		if (auto msg = validateJson(Globals::schema, json)) {
			msg->setTitle("Invalid request");
			sdk::utils::Logger::error(msg);

			nlohmann::json response;
			response["type"] = "response";
			response["nonce"] = json["nonce"];
			response["data"]["status"] = "error";
			response["data"]["error"] = "invalid_request";
			response["data"]["report"] = msg;
			sendJson(response);
			continue;
		}
		nlohmann::json response;
		response["type"] = "response";
		response["nonce"] = json["nonce"];
		auto data = json.at("data");
		if (auto type = json["type"].get<std::string>(); type == "command") {
			Command command = data.get<Command>();
			if (auto iter = commandParsers.find(command.getCommand()); iter != commandParsers.end()) {
				std::vector<const char*> args;
				for (const auto &arg: command.getArgs()) { args.emplace_back(arg.c_str()); }

				if (auto msg =
						iter->second->parse(static_cast<int>(args.size()), const_cast<char* const*>(args.data()))) {

					response["data"]["status"] = "error";
					response["data"]["error"] = "error_in_command";
					response["data"]["report"] = msg;
				} else {
					response["data"]["status"] = "success";
				}
			} else {
				response["data"]["status"] = "error";
				response["data"]["error"] = "unknown_command";
			}


		} else {
			response["data"]["status"] = "error";
			response["data"]["error"] = "unknown_type";
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to parse the request");
			msg->setMessage("Got unknown type");
			msg->addInfoLine("Specified type: {}", type);
			response["data"]["report"] = msg;
		}
		sendJson(response);
	}
}

void Client::sendJson(const nlohmann::json &pJson) {

	std::string str = pJson.dump(4) + "\n";

	sdk::utils::Logger::info("Written {}", str);
	std::error_code error;
	auto bytes = asio::write(socket, asio::buffer(str, str.size()), error);
	sdk::utils::Logger::info("Written {} bytes", bytes);
	if (error) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to send data");
		sdk::utils::Logger::error(msg);
	}
}

sdk::utils::ReportMessagePtr Client::addLib(const std::filesystem::path &pLibPath) {
	const std::string libPath = pLibPath.string();
	if (auto iter = loadedLibs.find(pLibPath.stem()); iter != loadedLibs.end()) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to open library");
		msg->setMessage("The specified library already opened");
		msg->addInfoLine("Path: {}", libPath);
		msg->addInfoLine("Error: {}", Utils::parseDlError(dlerror()));
		return msg;
	} else {
		auto editorLib = dlopen(libPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if (!editorLib) {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to open editor library");
			msg->setMessage("The function dlopen() returned error");
			msg->addInfoLine("Path: {}", libPath);
			msg->addInfoLine("Error: {}", Utils::parseDlError(dlerror()));
			return msg;
		}
		loadedLibs.emplace_hint(iter, pLibPath.stem(), editorLib);
	}
	return nullptr;
}


} // namespace mer::runner

void mer::sdk::utils::to_json(nlohmann::json &pNlohmannJsonJ, const ReportMessagePtr &pNlohmannJsonT) {
	pNlohmannJsonJ["title"] = pNlohmannJsonT->getTitle();
	pNlohmannJsonJ["message"] = pNlohmannJsonT->getMessage();
	if (auto ex = pNlohmannJsonT->getExceptionPtr()) {

		try {
			std::rethrow_exception(ex);
		} catch (std::exception &e) {
			pNlohmannJsonJ["exception"]["class"] = Utils::getTypeName(&e);
			pNlohmannJsonJ["exception"]["message"] = e.what();
		}
	}
	pNlohmannJsonJ["info-lines"] = pNlohmannJsonT->getInfoLines();
	auto stacktrace = pNlohmannJsonT->getStacktrace();
	auto array = nlohmann::json::array();
	for (const auto &stacktraceEntry: stacktrace) {
		nlohmann::json obj;
		obj["description"] = stacktraceEntry.description();
		obj["file"] = stacktraceEntry.source_file();
		obj["line"] = stacktraceEntry.source_line();
		array.emplace_back(obj);
	}
	pNlohmannJsonJ["stacktrace"] = array;
}