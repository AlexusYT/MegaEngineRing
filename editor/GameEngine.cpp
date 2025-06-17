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
// Created by alexus on 05.12.23.
//

#include "GameEngine.h"

#include <unordered_set>
#include <utility>

#if defined _WIN32
#include <windows.h>
#endif

#include <curl/curl.h>

#include "KwasarEngine/context/Application.h"
#include "KwasarEngine/context/Window.h"
#include "Globals.h"
#include "I18N.h"
#include "examples/libs/glfw/include/GLFW/glfw3.h"
#include "mvp/editor/EditorUi.h"
#include "mvp/editor/settings/Settings.h"
#include "mvp/onlineImport/ViewOnlineImport.h"
#include "mvp/sceneEditor/ViewSceneEditor.h"
#include "project/Project.h"

namespace mer::editor::ui {
class EditorWindow : public ke::Window {
public:
	explicit EditorWindow() { glfwWindowHint(GLFW_SAMPLES, 4); }

protected:
	ke::ReportMessagePtr init() override {
		auto mainUi = std::make_shared<mvp::EditorUi>();

		mainUi->addEditor(std::make_shared<mvp::OnlineImportWorkspace>());
		mainUi->addEditor(std::make_shared<mvp::SceneEditor>("Scene 1"));
		addScene(mainUi);

		auto project = project::Project::create();
		if (!project) return nullptr;
		project->setProjectName("Untitled");
		auto path = Globals::getProjectsPath() / "unsaved/Untitled";
		project->setProjectPath(path);
		create_directories(path);

		Window::init();

		return nullptr;
	}
};

int GameEngine::run(const int pArgc, char* pArgv[]) {
	Globals::init();
	mvp::Settings::init();
	auto application = ke::Application::create();

	application->initEngine();

	auto settingsPath = Globals::getConfigPath() / "settings.json";
	mvp::Settings::setSettingsPath(settingsPath);

	if (std::filesystem::exists(settingsPath)) {
		if (auto msg = mvp::Settings::load()) {
			ke::Logger::error(msg);
			ke::Logger::error("Using default settings as a fallback...");
			mvp::Settings::loadDefaults();
		} else
			ke::Logger::info("Settings are loaded successfully");
	} else {
		ke::Logger::info("Settings file does not exists. Creating with defaults...");
		mvp::Settings::loadDefaults();
		if (auto msg = mvp::Settings::save()) ke::Logger::error(msg);
	}
	mvp::I18n::init();


	if (auto ret = curl_global_init(CURL_GLOBAL_DEFAULT); ret != CURLE_OK) {
		auto msg = ke::ReportMessage::create();
		msg->setTitle("Failed to initialize CURL");
		msg->setMessage("Function curl_global_init returned an error");
		msg->addInfoLine("Error: {}", curl_easy_strerror(ret));
		ke::Logger::error(msg);
		return 1;
	}

	auto window = std::make_shared<EditorWindow>();
	application->registerWindow(window);


	auto result = application->runMainLoop(pArgc, pArgv);
	curl_global_cleanup();
	mvp::I18n::deinit();
	return result;
}
} // namespace mer::editor::ui