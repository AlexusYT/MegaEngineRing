//
// Created by alexus on 11.12.23.
//

#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

class ProjectSettings {

	inline static std::filesystem::path projectPath = "/home/alexus/GameEngineProjects/Test";

public:
	[[nodiscard]] static const std::filesystem::path &getProjectPath() { return projectPath; }
};


#endif //PROJECTSETTINGS_H
