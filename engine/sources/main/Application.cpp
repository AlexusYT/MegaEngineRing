//
// Created by alexus on 06.01.24.
//

#include "EngineSDK/main/Application.h"

#include <EngineUtils/utils/Logger.h>

#include "EngineSDK/main/DefaultApplicationSettings.h"

namespace n::sdk::main {
engine::utils::ReportMessageUPtr Application::initEngine() { return nullptr; }

int Application::runMainLoop(int argc, char* argv[]) {
	using namespace engine::utils;
	//read engine settings
	bool userSettingsLoaded = false;
	if (applicationSettings) {
		if (ReportMessageUPtr msg = applicationSettings->init()) {
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

	auto logPath = std::filesystem::path(applicationSettings->getLogsDirectory().getValue()) / "latest.log";
	if (auto msg = Logger::openLog(logPath)) {
		Logger::error("Log file cannot be open, so logging to file feature won't be available");
		Logger::error(msg);
	} else {
		Logger::info("Log opened at {}", logPath.string());
	}

	return 0;
}
} // namespace n::sdk::main