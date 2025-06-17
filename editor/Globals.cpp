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
// Created by alexus on 31.12.23.
//

#include "Globals.h"
namespace ked {
void Globals::init() {
#if defined __MINGW32__

	const auto local = getenv("LOCALAPPDATA");
	if (!local) throw std::runtime_error("Error while getting LOCALAPPDATA");
	cachePath = std::filesystem::path(local) / "MegaEngineRing";
	create_directories(cachePath);
	logPath = cachePath / "logs";
	create_directories(logPath);

	const auto home = getenv("USERPROFILE");
	if (!home) throw std::runtime_error("Error while getting USERPROFILE");
	projectsPath = std::filesystem::path(home) / "MegaEngineProjects";
	create_directories(projectsPath);

	const auto config = getenv("APPDATA");
	if (!config) throw std::runtime_error("Error while getting APPDATA");
	configPath = std::filesystem::path(config) / "MegaEngineRing";
	create_directories(configPath);

#else

	const std::filesystem::path home = getenv("HOME");
	logPath = home / ".cache/MegaEngineRing/logs";
	create_directories(logPath);
	projectsPath = home / "MegaEngineProjects";
	create_directories(projectsPath);
	configPath = home / ".config/MegaEngineRing";
	create_directories(configPath);
	cachePath = home / ".cache/MegaEngineRing/";
	create_directories(cachePath);
#endif
}
}