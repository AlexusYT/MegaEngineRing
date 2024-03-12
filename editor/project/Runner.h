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

#ifndef RUNNER_H
#define RUNNER_H
#include <nlohmann/json_fwd.hpp>
#include <thread>

// ReSharper disable CppInconsistentNaming


namespace asio {
class any_io_executor;
template<typename Protocol, typename Executor>
class basic_stream_socket;

namespace ip {
	class tcp;
} // namespace ip
} // namespace asio

// ReSharper restore CppInconsistentNaming

namespace mer::editor::project {
class Project;

class Runner {
	std::jthread runnerThread;
	Project* project;
	std::shared_ptr<asio::basic_stream_socket<asio::ip::tcp, asio::any_io_executor>> socket;
	std::chrono::milliseconds readTimeout;

	struct Request {
		int nonce{};
		sigc::slot<void(const nlohmann::json &pJson)> callbackFunc;
	};

	std::map<int, Request> requests;

	sigc::signal<void()> onRunnerConnected;

public:
	explicit Runner(Project* pProject);


	void readLoop(const std::stop_token &pToken);

	sdk::utils::ReportMessagePtr write(const nlohmann::json &pJson,
									   const sigc::slot<void(const nlohmann::json &pResponse)> &pFunction = {});

	void read(nlohmann::json &pJson);

	sdk::utils::ReportMessagePtr execute(const nlohmann::json &pRequest);

	sdk::utils::ReportMessagePtr loadCommand(const std::filesystem::path &pLibrary);

	sdk::utils::ReportMessagePtr runCommand(const std::string &pLibraryName, const std::string &pMethodName);

	[[nodiscard]] const std::chrono::milliseconds &getReadTimeout() const { return readTimeout; }

	void setReadTimeout(const std::chrono::milliseconds &pReadTimeout) { readTimeout = pReadTimeout; }

	[[nodiscard]] sigc::signal<void()> &getOnRunnerConnected() { return onRunnerConnected; }
};
} // namespace mer::editor::project


#endif //RUNNER_H
