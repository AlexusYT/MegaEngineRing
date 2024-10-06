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

#ifndef GLOBALS_H
#define GLOBALS_H

class Globals {
	inline static std::filesystem::path logPath;
	inline static std::filesystem::path projectsPath;
	inline static std::filesystem::path sdkPath;
	inline static std::string sdkVersion;
	inline static std::filesystem::path sdk;
	inline static std::filesystem::path resourcesPath;
	inline static std::filesystem::path dataPath;

public:
	static void init();

	[[nodiscard]] static const std::filesystem::path &getLogPath() { return logPath; }

	static void setLogPath(const std::filesystem::path &pLogPath) { logPath = pLogPath; }

	[[nodiscard]] static const std::filesystem::path &getProjectsPath() { return projectsPath; }

	static void setProjectsPath(const std::filesystem::path &pProjectsPath) { projectsPath = pProjectsPath; }

	[[nodiscard]] static const std::filesystem::path &getSdkPath() { return sdkPath; }

	static void setSdkPath(const std::filesystem::path &pSdkPath) { sdkPath = pSdkPath; }

	[[nodiscard]] static const std::string &getSdkVersion() { return sdkVersion; }

	static void setSdkVersion(const std::string &pSdkVersion) { sdkVersion = pSdkVersion; }

	[[nodiscard]] static const std::filesystem::path &getSdk() { return sdk; }

	static void setSdk(const std::filesystem::path &pSdk) { sdk = pSdk; }

	[[nodiscard]] static const std::filesystem::path &getResourcesPath() { return resourcesPath; }

	static void setResourcesPath(const std::filesystem::path &pResourcesPath) { resourcesPath = pResourcesPath; }

	[[nodiscard]] static const std::filesystem::path &getDataPath() { return dataPath; }

	static void setDataPath(const std::filesystem::path &pDataPath) { dataPath = pDataPath; }
};


#endif //GLOBALS_H
