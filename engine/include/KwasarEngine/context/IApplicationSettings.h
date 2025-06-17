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

#ifndef IAPPLICATIONSETTINGS_H
#define IAPPLICATIONSETTINGS_H

#include <KwasarEngine/utils/Property.h>

namespace ke {
class IApplicationSettings {
public:
	IApplicationSettings() = default;

	virtual ~IApplicationSettings() = default;

	virtual ke::ReportMessagePtr init() = 0;

	[[nodiscard]] virtual const Property<std::string> &getLogsDirectory() const = 0;

	virtual void setLogsDirectory(const std::string &pLogsDirectory) = 0;

	[[nodiscard]] virtual Property<std::string> &getApplicationName() = 0;

	virtual void setApplicationName(const std::string &pApplicationName) = 0;

	[[nodiscard]] virtual const Property<std::string> &getApplicationDisplayName() const = 0;

	virtual void setApplicationDisplayName(const std::string &pApplicationDisplayName) = 0;

	[[nodiscard]] virtual const Property<std::string> &getRunDirectory() const = 0;

	virtual void setRunDirectory(const std::string &pRunDirectory) = 0;
};
} // namespace ke

#endif //IAPPLICATIONSETTINGS_H
