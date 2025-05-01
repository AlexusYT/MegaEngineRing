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

#include "EngineSDK/context/Application.h"
#include "EngineSDK/context/Window.h"
#include "Globals.h"
#include "examples/libs/glfw/include/GLFW/glfw3.h"
#include "mvp/editor/EditorUi.h"
#include "mvp/onlineImport/ViewOnlineImport.h"
#include "mvp/sceneEditor/ViewSceneEditor.h"
#include "project/Project.h"

namespace mer::editor::ui {

class EditorWindow : public sdk::Window {

public:
	explicit EditorWindow() { glfwWindowHint(GLFW_SAMPLES, 4); }

protected:
	sdk::ReportMessagePtr init() override {
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
	auto application = sdk::Application::create();

	application->initEngine();
	auto window = std::make_shared<EditorWindow>();
	application->registerWindow(window);


	auto result = application->runMainLoop(pArgc, pArgv);
	return result;
}

} // namespace mer::editor::ui