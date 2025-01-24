//  MegaEngineRing is a program that can speed up game development.
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

#include <EngineSDK/context/DefaultApplicationSettings.h>

namespace mer::sdk {
DefaultApplicationSettings::DefaultApplicationSettings()
	: applicationName(nullptr, "ApplicationName"), applicationDisplayName(nullptr, "AppDisplayName"),
	  logsDirectory(nullptr, "LogsDir"), runDirectory(nullptr, "RunDir") {}

sdk::ReportMessagePtr DefaultApplicationSettings::init() {
	if (applicationName->empty()) setApplicationName("DefaultApplication");
	if (applicationDisplayName->empty()) setApplicationDisplayName("Default Application");
	/*if (logsDirectory->empty())
		setLogsDirectory(std::filesystem::path(getenv("HOME")) / getApplicationName().getValue() / "Logs");*/
	//setLogsDirectory("{HOME}/{APPLICATION_NAME}/Logs");
	runDirectory = std::filesystem::current_path();
	return nullptr;
}
} // namespace mer::sdk
