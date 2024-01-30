//
// Created by alexus on 06.01.24.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include <EngineUtils/utils/ReportMessage.h>

#include "IApplicationSettings.h"

namespace n::sdk::main {
class Application {
	std::shared_ptr<IApplicationSettings> applicationSettings;

public:
	engine::utils::ReportMessageUPtr initEngine();

	int runMainLoop(int argc, char* argv[]);

	[[nodiscard]] const std::shared_ptr<IApplicationSettings> &getApplicationSettings() const {
		return applicationSettings;
	}

	void setApplicationSettings(const std::shared_ptr<IApplicationSettings> &pApplicationSettings) {
		applicationSettings = pApplicationSettings;
	}
};
} // namespace n::sdk::main


#endif //APPLICATION_H
