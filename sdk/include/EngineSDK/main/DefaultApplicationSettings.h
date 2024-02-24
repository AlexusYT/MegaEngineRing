//
// Created by alexus on 19.01.24.
//

#ifndef DEFAULTAPPLICATIONSETTINGS_H
#define DEFAULTAPPLICATIONSETTINGS_H

#include <EngineUtils/utils/Logger.h>
#include <EngineUtils/utils/ReportMessage.h>

#include "IApplicationSettings.h"

namespace mer::sdk::main {

class DefaultApplicationSettings : public IApplicationSettings {


	/**
	 * @brief The application name. Used in paths.
	 */
	utils::Property<std::string> applicationName;

	/**
	 * @brief The application display name. Used to display the application name to user.
	 */
	utils::Property<std::string> applicationDisplayName;

	/**
	 * @brief The directory where the logs will be stored.
	 */
	utils::Property<std::string> logsDirectory;

public:
	DefaultApplicationSettings() = default;

	//TODO make replacing of the environment variables
	sdk::utils::ReportMessagePtr init() override;

	[[nodiscard]] const utils::Property<std::string> &getLogsDirectory() const override { return logsDirectory; }

	void setLogsDirectory(const std::string &pLogsDirectory) override { logsDirectory = pLogsDirectory;
		sdk::utils::Logger::info(pLogsDirectory);}

	[[nodiscard]] utils::Property<std::string> &getApplicationName() override { return applicationName; }

	void setApplicationName(const std::string &pApplicationName) override { applicationName = pApplicationName; }

	[[nodiscard]] const utils::Property<std::string> &getApplicationDisplayName() const override {
		return applicationDisplayName;
	}

	void setApplicationDisplayName(const std::string &pApplicationDisplayName) override {
		applicationDisplayName = pApplicationDisplayName;
	}

private:
	/*std::string parse(const std::string &input) {
		std::string output = input;
		std::forma
	}*/
};

} // namespace mer::sdk::main

#endif //DEFAULTAPPLICATIONSETTINGS_H
