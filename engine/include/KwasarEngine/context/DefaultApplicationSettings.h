//  KwasarEngine is an SDK that can help you speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 19.01.24.
//

#ifndef DEFAULTAPPLICATIONSETTINGS_H
#define DEFAULTAPPLICATIONSETTINGS_H

#include <KwasarEngine/utils/Logger.h>
#include <KwasarEngine/utils/ReportMessage.h>

#include "IApplicationSettings.h"

namespace ke {
class DefaultApplicationSettings : public IApplicationSettings {
	/**
	 * @brief The application name. Used in paths.
	 */
	Property<std::string> applicationName;

	/**
	 * @brief The application display name. Used to display the application name to user.
	 */
	Property<std::string> applicationDisplayName;

	/**
	 * @brief The directory where the logs will be stored.
	 */
	Property<std::string> logsDirectory;

	/**
	 * @brief The directory where the app running.
	 */
	Property<std::string> runDirectory;

public:
	DefaultApplicationSettings();

	//TODO make replacing of the environment variables
	ke::ReportMessagePtr init() override;

	[[nodiscard]] const Property<std::string> &getLogsDirectory() const override { return logsDirectory; }

	void setLogsDirectory(const std::string &pLogsDirectory) override {
		logsDirectory = pLogsDirectory;
		ke::Logger::info(pLogsDirectory);
	}

	[[nodiscard]] Property<std::string> &getApplicationName() override { return applicationName; }

	void setApplicationName(const std::string &pApplicationName) override { applicationName = pApplicationName; }

	[[nodiscard]] const Property<std::string> &getApplicationDisplayName() const override {
		return applicationDisplayName;
	}

	void setApplicationDisplayName(const std::string &pApplicationDisplayName) override {
		applicationDisplayName = pApplicationDisplayName;
	}

	[[nodiscard]] const Property<std::string> &getRunDirectory() const override { return runDirectory; }

	void setRunDirectory(const std::string &pRunDirectory) override { runDirectory.setValue(pRunDirectory); }

private:
	/*std::string parse(const std::string &input) {
		std::string output = input;
		std::forma
	}*/
};
} // namespace ke

#endif //DEFAULTAPPLICATIONSETTINGS_H
