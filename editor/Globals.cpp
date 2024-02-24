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

	sdkPath = std::filesystem::path(SDK_PATH) / "sdk";

	resourcesPath = std::filesystem::path(RESOURCE_PATH) / "Resources";
}
