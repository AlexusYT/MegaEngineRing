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

#ifndef IAPPLICATIONSETTINGS_H
#define IAPPLICATIONSETTINGS_H

#include <EngineSDK/utils/Property.h>

namespace mer::sdk::main {

class IApplicationSettings {
public:
	IApplicationSettings() = default;
	virtual ~IApplicationSettings() = default;

	virtual sdk::utils::ReportMessagePtr init() = 0;

	[[nodiscard]] virtual const utils::Property<std::string> &getLogsDirectory() const = 0;

	virtual void setLogsDirectory(const std::string &pLogsDirectory) = 0;

	[[nodiscard]] virtual utils::Property<std::string> &getApplicationName() = 0;

	virtual void setApplicationName(const std::string &pApplicationName) = 0;

	[[nodiscard]] virtual const utils::Property<std::string> &getApplicationDisplayName() const = 0;

	virtual void setApplicationDisplayName(const std::string &pApplicationDisplayName) = 0;
};

} // namespace mer::sdk::main

#endif //IAPPLICATIONSETTINGS_H
