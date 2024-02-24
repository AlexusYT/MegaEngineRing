//
// Created by alexus on 19.01.24.
//

#ifndef IAPPLICATIONSETTINGS_H
#define IAPPLICATIONSETTINGS_H

#include <EngineSDK/utils/Property.h>

namespace mer::sdk::main {

class IApplicationSettings {
public:
	IApplicationSettings() = default;
	virtual ~IApplicationSettings() = default;

	virtual sdk::utils::ReportMessagePtr init() = 0;

	[[nodiscard]] virtual const utils::Property<std::string> &getLogsDirectory() const = 0;

	virtual void setLogsDirectory(const std::string &pLogsDirectory) = 0;

	[[nodiscard]] virtual utils::Property<std::string> &getApplicationName() = 0;

	virtual void setApplicationName(const std::string &pApplicationName) = 0;

	[[nodiscard]] virtual const utils::Property<std::string> &getApplicationDisplayName() const = 0;

	virtual void setApplicationDisplayName(const std::string &pApplicationDisplayName) = 0;
};

} // namespace mer::sdk::main

#endif //IAPPLICATIONSETTINGS_H
