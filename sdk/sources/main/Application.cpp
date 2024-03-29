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
// Created by alexus on 06.01.24.
//

#ifndef EDITOR_SDK
	#include "EngineSDK/main/Application.h"

	#include <EngineUtils/utils/Logger.h>
	#include <GLFW/glfw3.h>
	#include <signal.h>

	#include "EngineSDK/main/DefaultApplicationSettings.h"
	#include "EngineSDK/main/context/MainWindow.h"
	#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"

extern std::shared_ptr<mer::sdk::main::IScene> getPrimaryScene();

namespace mer::sdk::main {
sdk::utils::ReportMessagePtr Application::initEngine() { return nullptr; }

int Application::runMainLoop(int /*argc*/, char* /*argv*/[]) {
	using namespace sdk::utils;
	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = [](int pSig, siginfo_t* pInfo, void*) {
		auto msg = ReportMessage::create();
		msg->setTitle("Signal recieved");
		msg->setMessage("Aborting...");
		std::string sigName;
		switch (pSig) {
			case SIGILL: sigName = "SIGILL"; break;
			case SIGSEGV: sigName = "SIGSEGV"; break;
			default: sigName = "Unknown"; break;
		}
		msg->addInfoLine("Signal: {}", sigName);
		msg->addInfoLine("Code: {}", pInfo->si_code);
		Logger::error(msg);
		exit(1);
	};
	sigaction(SIGILL, &sig, nullptr);
	sigaction(SIGSEGV, &sig, nullptr);
	bool userSettingsLoaded = false;
	if (applicationSettings) {
		if (ReportMessagePtr msg = applicationSettings->init()) {
			Logger::error("Failed to load user-defined ApplicationSettings:");
			Logger::error(msg);
		} else {
			userSettingsLoaded = true;
			Logger::info("User-defined ApplicationSettings has been loaded successfully");
		}
	}
	if (!userSettingsLoaded) {
		Logger::warn("Loading default ApplicationSettings");
		applicationSettings = std::make_shared<DefaultApplicationSettings>();
		applicationSettings->init();
	}

	if (const auto logsDir = applicationSettings->getLogsDirectory().getValue(); !logsDir.empty()) {
		const auto logPath = std::filesystem::path(logsDir) / "latest.log";
		if (const auto msg = Logger::openLog(logPath)) {
			Logger::error("Log file cannot be open, so logging to file feature won't be available");
			Logger::error(msg);
		} else {
			Logger::info("Log opened at {}", logPath.string());
		}
	} else {
		Logger::info("LogsDirectory property is empty. Logging to file will be disabled.");
	}


	if (!glfwInit()) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to init engine");
		msg->setMessage("Failed to init glfw");
		Logger::error(msg);
		return 1;
	}

	glfwSetErrorCallback([](int error_code, const char* description) {
		auto msg = ReportMessage::create();
		msg->setTitle("GLFW error");
		msg->setMessage("GLFW error");
		msg->addInfoLine("Error code: {}", error_code);
		msg->addInfoLine("Error description: {}", description);
		Logger::error(msg);
	});


	std::shared_ptr<MainWindow> window = MainWindow::create();
	if (auto msg = window->setContextVersion(4, 0)) {
		Logger::error(msg);
		return 1;
	}
	window->show();

	if (const auto msg = window->getContext()->init()) {
		Logger::error(msg);
		return 1;
	}

	ExtensionRegistry::init();
	Logger::out("Opening primary scene");
	window->openScene(getPrimaryScene());

	Logger::info("Starting main loop...");
	try {
		window->runMainLoop();
	} catch (...) {
		auto msg = ReportMessage::create();
		msg->setTitle("Exception occurred in main loop");
		msg->setMessage("Exception occurred in main loop");
		Logger::error(msg);
	}
	Logger::info("Main loop finished");
	ExtensionRegistry::deinit();

	return 0;
}
} // namespace mer::sdk::main
#endif