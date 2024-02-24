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
};


#endif //GLOBALS_H
