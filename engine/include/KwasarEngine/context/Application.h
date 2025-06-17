//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 06.01.24.
//

#ifndef APPLICATION_H
#define APPLICATION_H


#include <KwasarEngine/utils/ReportMessage.h>

#include "IApplication.h"
#include "IApplicationSettings.h"

namespace ke {
class Window;

class Application : public IApplication {
	std::shared_ptr<IApplicationSettings> applicationSettings;
	std::shared_ptr<IResourceBundle> resourceBundle;
	std::shared_ptr<Window> window;

	Application() = default;

public:
	static std::shared_ptr<Application> create();

	ReportMessagePtr initEngine() override;

	void deinitEngine() override;

	int runMainLoop(int argc, char* argv[]) override;

	[[nodiscard]] const std::shared_ptr<IApplicationSettings> &getApplicationSettings() const override {
		return applicationSettings;
	}

	void setApplicationSettings(const std::shared_ptr<IApplicationSettings> &pApplicationSettings) override {
		applicationSettings = pApplicationSettings;
	}

	[[nodiscard]] const std::shared_ptr<IResourceBundle> &getResourceBundle() const override { return resourceBundle; }

	void setResourceBundle(const std::shared_ptr<IResourceBundle> &pResourceBundle) override {
		resourceBundle = pResourceBundle;
	}

	void registerWindow(const std::shared_ptr<Window> &pWindow);

private:
	static void sigHandler(int pSig);

	static void initSigHandlers();

	void loadSettings();

	void createLog() const;

	ReportMessagePtr setupGlfw();
};
} // namespace ke


#endif //APPLICATION_H