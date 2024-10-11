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
// Created by alexus on 31.12.23.
//

#include "Globals.h"

#include "Resources.h"

void Globals::init() {
	const std::filesystem::path home = getenv("HOME");
	logPath = home / ".cache/MegaEngineRing/logs";
	create_directories(logPath);
	projectsPath = home / "MegaEngineProjects";
	create_directories(projectsPath);
	configPath = home / ".config/MegaEngineRing";
	create_directories(configPath);

	sdkPath = std::filesystem::path(SDK_PATH) / "sdk";

	dataPath = std::filesystem::path(RESOURCE_PATH) / "data";
	resourcesPath = std::filesystem::path(RESOURCE_PATH) / "Resources";
}
