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

	/**
	 * @brief The directory where the app running.
	 */
	utils::Property<std::string> runDirectory;

public:
	DefaultApplicationSettings() = default;

	//TODO make replacing of the environment variables
	sdk::utils::ReportMessagePtr init() override;

	[[nodiscard]] const utils::Property<std::string> &getLogsDirectory() const override { return logsDirectory; }

	void setLogsDirectory(const std::string &pLogsDirectory) override {
		logsDirectory = pLogsDirectory;
		sdk::utils::Logger::info(pLogsDirectory);
	}

	[[nodiscard]] utils::Property<std::string> &getApplicationName() override { return applicationName; }

	void setApplicationName(const std::string &pApplicationName) override { applicationName = pApplicationName; }

	[[nodiscard]] const utils::Property<std::string> &getApplicationDisplayName() const override {
		return applicationDisplayName;
	}

	void setApplicationDisplayName(const std::string &pApplicationDisplayName) override {
		applicationDisplayName = pApplicationDisplayName;
	}

	[[nodiscard]] const utils::Property<std::string> &getRunDirectory() const override { return runDirectory; }

	void setRunDirectory(const std::string &pRunDirectory) override { runDirectory.setValue(pRunDirectory); }

private:
	/*std::string parse(const std::string &input) {
		std::string output = input;
		std::forma
	}*/
};

} // namespace mer::sdk::main

#endif //DEFAULTAPPLICATIONSETTINGS_H
