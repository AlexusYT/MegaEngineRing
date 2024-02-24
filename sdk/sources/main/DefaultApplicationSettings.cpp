//
// Created by alexus on 19.01.24.
//

#include <EngineSDK/main/DefaultApplicationSettings.h>

namespace mer::sdk::main {
sdk::utils::ReportMessagePtr DefaultApplicationSettings::init() {
	if (applicationName->empty()) setApplicationName("DefaultApplication");
	if (applicationDisplayName->empty()) setApplicationDisplayName("Default Application");
	/*if (logsDirectory->empty())
		setLogsDirectory(std::filesystem::path(getenv("HOME")) / getApplicationName().getValue() / "Logs");*/
	//setLogsDirectory("{HOME}/{APPLICATION_NAME}/Logs");
	return nullptr;
}
} // namespace mer::sdk::main
