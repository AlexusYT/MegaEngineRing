//
// Created by alexus on 11.12.23.
//

#ifndef TOOLCHAINSETTINGS_H
#define TOOLCHAINSETTINGS_H

class ToolchainSettings {
	inline static std::filesystem::path cmakePath =
		"/home/alexus/.local/share/JetBrains/Toolbox/apps/clion-nova/bin/cmake/linux/x64/bin/cmake";
	inline static std::filesystem::path vcpkgPath = "/home/alexus/.clion-vcpkg/vcpkg/vcpkg";

public:
	static void setCMakePath(const std::filesystem::path &pNewPath) { cmakePath = pNewPath; }

	[[nodiscard]] static const std::filesystem::path &getCMakePath() { return cmakePath; }

	[[nodiscard]] static const std::filesystem::path &getVcpkgPath() { return vcpkgPath; }

	static void setVcpkgPath(const std::filesystem::path &pVcpkgPath) { vcpkgPath = pVcpkgPath; }
};


#endif //TOOLCHAINSETTINGS_H
