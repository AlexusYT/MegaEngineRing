//
// Created by alexus on 31.12.23.
//

#ifndef GLOBALS_H
#define GLOBALS_H

class Globals {
	inline static std::filesystem::path logPath;
	inline static std::filesystem::path projectsPath;

public:
	static void init() {
		const std::filesystem::path home = getenv("HOME");
		logPath = home / ".cache/GameEngine/logs";
		create_directories(logPath);
		projectsPath = home / "GameEngineProjects";
		create_directories(projectsPath);
	}

	[[nodiscard]] static const std::filesystem::path &getLogPath() { return logPath; }

	static void setLogPath(const std::filesystem::path &pLogPath) { logPath = pLogPath; }

	[[nodiscard]] static const std::filesystem::path &getProjectsPath() { return projectsPath; }

	static void setProjectsPath(const std::filesystem::path &pProjectsPath) { projectsPath = pProjectsPath; }
};


#endif //GLOBALS_H
