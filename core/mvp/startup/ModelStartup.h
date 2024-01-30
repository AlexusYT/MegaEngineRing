//
// Created by alexus on 25.01.24.
//

#ifndef MODELSTARTUP_H
#define MODELSTARTUP_H

#include <mvp/startup/IModelStartup.h>

namespace MVP_CORE {
class ModelStartup : public IModelStartup {
	std::string name;
	std::filesystem::path path;

public:
	ModelStartup();

	[[nodiscard]] const std::string &getName() const override { return name; }

	void setName(const std::string &pName) override { name = pName; }

	[[nodiscard]] const std::filesystem::path &getPath() const override { return path; }

	void setPath(const std::filesystem::path &pPath) override { path = pPath; }
};
} // namespace MVP_CORE

#endif //MODELSTARTUP_H
