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
// Created by alexus on 23.06.24.
//

#ifndef IAPPLICATION_H
#define IAPPLICATION_H

#include "EngineSDK/utils/ReportMessageFwd.h"

namespace mer::sdk {
class IResourceBundle;
class IApplicationSettings;

class IApplication {
public:
	virtual ~IApplication() = default;

	virtual ReportMessagePtr initEngine() = 0;

	virtual void deinitEngine() = 0;

	virtual int runMainLoop(int argc, char* argv[]) = 0;

	[[nodiscard]] virtual const std::shared_ptr<IApplicationSettings> &getApplicationSettings() const = 0;

	virtual void setApplicationSettings(const std::shared_ptr<IApplicationSettings> &pApplicationSettings) = 0;

	[[nodiscard]] virtual const std::shared_ptr<IResourceBundle> &getResourceBundle() const = 0;

	virtual void setResourceBundle(const std::shared_ptr<IResourceBundle> &pResourceBundle) = 0;
};
} // namespace mer::sdk


#endif //IAPPLICATION_H
