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
// Created by alexus on 06.03.24.
//

#include "Runner.h"

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read_until.hpp>
#include <asio/streambuf.hpp>
#include <asio/write.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Globals.h"
#include "Project.h"
#include "toolchain/ToolchainUtils.h"

namespace mer::editor::project {

Runner::Runner(Project* pProject)
	: runnerThread([this](const std::stop_token &pToken) { this->readLoop(pToken); }), project(pProject) {
	using namespace std::chrono_literals;
	readTimeout = 5s;
}

void Runner::readLoop(const std::stop_token &pToken) {
	asio::io_context ioContext;
	asio::ip::tcp::resolver resolver(ioContext);
	socket = std::make_shared<asio::ip::tcp::socket>(ioContext);
	const asio::ip::address IP(asio::ip::make_address("127.0.0.1"));
	auto endpoint = asio::ip::tcp::endpoint(IP, 2736);
	while (!pToken.stop_requested()) {
		{
			sdk::utils::Logger::info("Starting runner");
			auto path = std::filesystem::current_path();
			ToolchainUtils::execute(path, path / "MegaEngineRunner", "",
									[](const std::string & /*pLog*/) { /*sdk::utils::Logger::info(pLog);*/ }, {});
		}
		sdk::utils::Logger::info("Waiting for runner to connect");
		while (true) {
			try {
				if (socket) {
					socket->connect(endpoint);
					sdk::utils::Logger::info("Connected to runner");
					std::thread([this] { onRunnerConnected(); }).detach();
					break;
				}
			} catch (...) {}
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(10ms);
		}
		while (!pToken.stop_requested()) {
			try {
				nlohmann::json json;
				read(json);
			} catch (std::system_error &e) {
				if (e.code() == asio::error::eof) {
					sdk::utils::Logger::warn("End of file. Runner crashed.");
					break;
				}
			}
		}
	}
}

sdk::utils::ReportMessagePtr Runner::write(const nlohmann::json &pJson,
										   const sigc::slot<void(const nlohmann::json &pResponse)> &pFunction) {
	static int nonce = 0;
	auto json = pJson;
	json["nonce"] = nonce;
	std::string str = json.dump(4);

	std::error_code error;
	auto bytes = asio::write(*socket, asio::buffer(str, str.size()), error);
	if (error) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Failed to send data");
		msg->setMessage("Network error occurred");
		msg->addInfoLine("Error: {}", error.message());
		msg->addInfoLine("Data nonce: {}", nonce);
		return msg;
	}
	requests.emplace(nonce, Request{nonce, pFunction});
	nonce++;
	sdk::utils::Logger::out("Written {} bytes: {}", bytes, str);
	return nullptr;
}

void Runner::read(nlohmann::json &pJson) {


	size_t bytes;
	std::error_code error;
	asio::streambuf buff;
	bytes = asio::read_until(*socket, buff, "\n}\n", error);
	if (error == asio::error::eof) { sdk::utils::Logger::out("Connection closed cleanly by peer"); }
	if (error) throw std::system_error(error);
	buff.commit(bytes);
	std::istream is(&buff);


	sdk::utils::Logger::out("Read {} bytes: ", bytes);
	is >> pJson;
	sdk::utils::Logger::out("Json: {}", pJson.dump(4));
	if (int nonce = pJson["nonce"]; nonce >= 0) {
		if (auto iter = requests.find(nonce); iter != requests.end()) {
			iter->second.callbackFunc(pJson);
			requests.erase(iter);
		}
	} else {
		for (const auto &request: requests) { request.second.callbackFunc(pJson); }
	}
}

sdk::utils::ReportMessagePtr Runner::execute(const nlohmann::json &pRequest) {
	std::promise<sdk::utils::ReportMessagePtr> promise;
	auto future = promise.get_future();
	if (auto msg = write(pRequest, [&promise](const nlohmann::json &pResponse) {
			if (pResponse["type"] != "response") return;
			auto data = pResponse["data"];
			if (data["status"] == "error") { sdk::utils::Logger::error(data["report"].dump(4)); }
			promise.set_value(nullptr);
		}))
		return msg;

	switch (future.wait_for(readTimeout)) {

		case std::future_status::ready: return future.get();
		case std::future_status::timeout: {
			auto msg = sdk::utils::ReportMessage::create();
			msg->setTitle("Failed to read data");
			msg->setMessage("Timed out");
			return msg;
		}
		default: break;
	}
	return nullptr;
}

sdk::utils::ReportMessagePtr Runner::loadCommand(const std::filesystem::path &pLibrary) {

	nlohmann::json json;
	json["type"] = "command";
	json["data"]["command"] = "lib";
	json["data"]["args"] = {"load", pLibrary};
	return execute(json);
}

sdk::utils::ReportMessagePtr Runner::runCommand(const std::string & /*pLibraryName*/,
												const std::string & /*pMethodName*/) {

	nlohmann::json json;
	json["type"] = "command";
	json["data"]["command"] = "run";
	json["data"]["args"] = {""};
	return execute(json);
}
} // namespace mer::editor::project