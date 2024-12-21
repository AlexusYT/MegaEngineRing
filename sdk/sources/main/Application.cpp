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

#include "EngineSDK/main/Application.h"

#include <EngineUtils/utils/Logger.h>
#ifndef EDITOR_SDK
	#include <GLFW/glfw3.h>
#endif
#include <signal.h>

#include "EngineSDK/main/DefaultApplicationSettings.h"
#ifndef EDITOR_SDK
	#include "EngineSDK/main/context/MainWindow.h"
#endif
#include "EngineSDK/main/resources/ResourceLoaders.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"

#ifndef EDITOR_SDK
extern std::shared_ptr<mer::sdk::main::IScene> getPrimaryScene();
#endif

namespace mer::sdk::main {
std::shared_ptr<Application> Application::create() { return std::shared_ptr<Application>(new Application()); }

utils::ReportMessagePtr Application::initEngine() {

#ifndef EDITOR_SDK
	initSigHandlers();
#endif
	loadSettings();
#ifndef EDITOR_SDK
	createLog();
	if (auto msg = setupGlfw()) return msg;
#else
	ExtensionRegistry::init();
	ResourceLoaders::getInstance();
#endif
	return nullptr;
}

void Application::deinitEngine() {
	ExtensionRegistry::deinit();
	ResourceLoaders::clearLoaders();
}

void Application::sigHandler(int pSig) {
	using namespace sdk::utils;
	const auto msg = ReportMessage::create();
	msg->setTitle("Signal recieved");
	msg->setMessage("Aborting...");
	std::string sigName;
	switch (pSig) {
		case SIGILL: sigName = "SIGILL"; break;
		case SIGSEGV: sigName = "SIGSEGV"; break;
		default: sigName = "Unknown"; break;
	}
	msg->addInfoLine("Signal: {}", sigName);
	msg->addInfoLine("Code: {}", pSig);
	Logger::error(msg);
	exit(1);
}

void Application::initSigHandlers() {
#if defined __MINGW32__
	signal(SIGILL, &sigHandler);
	signal(SIGSEGV, &sigHandler);

#else

	struct sigaction sig;
	sig.sa_flags = SA_SIGINFO;
	sig.sa_sigaction = [](int pSig, siginfo_t* /*pInfo*/, void*) { sigHandler(pSig); };
	sigaction(SIGILL, &sig, nullptr);
	sigaction(SIGSEGV, &sig, nullptr);

#endif
}

void Application::loadSettings() {
	using namespace sdk::utils;
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
}

void Application::createLog() const {
	using namespace sdk::utils;
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
}
#ifndef EDITOR_SDK
utils::ReportMessagePtr Application::setupGlfw() {
	using namespace sdk::utils;
	if (!glfwInit()) {
		auto msg = ReportMessage::create();
		msg->setTitle("Failed to init engine");
		msg->setMessage("Failed to init glfw");
		Logger::error(msg);
		return msg;
	}

	glfwSetErrorCallback([](int pErrorCode, const char* pDescription) {
		auto msg = ReportMessage::create();
		msg->setTitle("GLFW error");
		msg->setMessage("GLFW error");
		msg->addInfoLine("Error code: {}", pErrorCode);
		msg->addInfoLine("Error description: {}", pDescription);
		Logger::error(msg);
	});
	return nullptr;
}
#endif
int Application::runMainLoop(int /*argc*/, char* /*argv*/[]) {
#ifndef EDITOR_SDK
	using namespace sdk::utils;

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

	window->setApplication(this);
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

#endif
	return 0;
}
} // namespace mer::sdk::main