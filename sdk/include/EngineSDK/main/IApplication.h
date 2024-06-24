//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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

namespace mer::sdk::main {
class IApplicationSettings;

class IApplication {

public:
	virtual ~IApplication() = default;

	virtual utils::ReportMessagePtr initEngine() = 0;

	virtual void deinitEngine() = 0;

#ifndef EDITOR_SDK
	virtual int runMainLoop(int argc, char* argv[]) = 0;
#endif

	[[nodiscard]] virtual const std::shared_ptr<IApplicationSettings> &getApplicationSettings() const = 0;

	virtual void setApplicationSettings(const std::shared_ptr<IApplicationSettings> &pApplicationSettings) = 0;
};
} // namespace mer::sdk::main


#endif //IAPPLICATION_H
