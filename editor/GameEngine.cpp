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

#include <mvp/startup/ModelStartup.h>
#include <mvp/startup/PresenterStartup.h>
#include <unordered_set>
#include <utility>

#if defined _WIN32
	#include <windows.h>
#endif
#include <imgui.h>
#include <thread>

#include "EngineSDK/context/Application.h"
#include "EngineSDK/context/Window.h"
#include "EngineSDK/scene/SceneUi.h"
#include "EngineSDK/ui/UiWindow.h"
#include "Globals.h"
#include "cmdOptions/EngineOptionGroup.h"
#include "examples/libs/glfw/include/GLFW/glfw3.h"
#include "imgui_internal.h"
#include "mvp/ApplicationController.h"
#include "mvp/IApplicationController.h"
#include "mvp/contexts/ApplicationContext.h"
#include "mvp/contexts/UiWindowContext.h"
#include "mvp/editor/EditorUi.h"
#include "mvp/main/MainWindow.h"
#include "mvp/main/ModelMain.h"
#include "mvp/main/PresenterMain.h"
#include "mvp/main/editors/sceneEditor/ViewSceneEditor.h"
#include "mvp/onlineImport/ViewOnlineImport.h"
#include "mvp/startup/StartupWindow.h"
#include "project/Project.h"

namespace mer::editor::ui {
class MainUiWindow : public sdk::UiWindow {
public:
	MainUiWindow() : UiWindow("MainWindow", "Main Window") {
		setWindowFlags(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
		setDockSpace();
		setSize({SIZE_MATH_PARENT, SIZE_MATH_PARENT});
	}

	void updateUi() override {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::SeparatorText("Project");
				if (ImGui::MenuItem("New")) {}
				if (ImGui::MenuItem("Open")) {}
				if (ImGui::MenuItem("Recent")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Text("This is some useful text.");
		static int counter = 0;
		if (ImGui::Button("Button")) counter++;
		ImGui::SameLine();
		const auto &io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", static_cast<double>(1000.0f / io.Framerate),
					static_cast<double>(io.Framerate));
	}
};

class StartupUiWindow : public sdk::UiWindow {
public:
	StartupUiWindow() : UiWindow("StartupWindow", "Startup Window") {
		setWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
					   ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNav);
		setPosition({POSITION_CENTERED, POSITION_CENTERED});
	}

	void updateUi() override {

		ImGui::Text("This is some useful text.");

		static int counter = 0;
		if (ImGui::Button("Button")) counter++;
		ImGui::SameLine();

		const auto &io = ImGui::GetIO();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", static_cast<double>(1000.0f / io.Framerate),
					static_cast<double>(io.Framerate));
	}
};

class GameEngineImpl {
	static inline std::shared_ptr<mvp::IApplicationController> appController;
	static inline std::unordered_set<std::shared_ptr<mvp::IPresenter>> windows;


public:
	static void runImgui() {}

	static int run(int pArgc, char* pArgv[]) {
#if defined __linux
		setenv("GDK_DEBUG", "gl-prefer-gl", false);
#endif
		Globals::init();
		signal(SIGABRT, signals);
		signal(SIGSEGV, signals);

		appController = std::make_shared<mvp::ApplicationController>();
		std::shared_ptr<Gtk::Application> application = Gtk::Application::create("org.gtkmm.example");

		appController->setApp(application);
		EngineOptionGroup group;
		application->add_option_group(group);

		application->signal_activate().connect(&GameEngineImpl::startup);
		application->register_application();
		auto result = application->run(pArgc, pArgv);
		windows.clear();
		appController.reset();
		return result;
	}

	static void signals(int pSig) {
		auto msg = sdk::ReportMessage::create();
		msg->setTitle("Signal handled");
		msg->setMessage("Aborting...");
		msg->addInfoLine("Signal: {}", pSig);
		sdk::Logger::error(msg);
		std::exit(-1);
	}

	static void startup() {
		using namespace mer::sdk;
		const std::filesystem::path logPath = Globals::getLogPath() / "latest.log";
		if (auto message = Logger::openLog(logPath)) {
			Logger::error("Log file cannot be open, so logging to file feature won't be available");
			Logger::error(message);
		} else {
			Logger::info("Log opened at {}", logPath.string());
		}
		Logger::info("SYSTEM INFO");
		Glib::add_exception_handler([]() {
			auto msg = ReportMessage::create();
			msg->setTitle("Exception occurred");
			msg->setMessage("Exception in the signal handler occurred");
			Logger::error(msg);
		});

		const std::shared_ptr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
		cssProvider->signal_parsing_error().connect(
			[](const Glib::RefPtr<const Gtk::CssSection> & /*section*/, const Glib::Error &pError) {
				//section.
				Logger::error(pError.what());
			},
			true);
		try {
			cssProvider->load_from_resource("css/style.css");
		} catch (Glib::Error &e) { Logger::error("Failed to load style {}", e.what()); }
		auto display = Gdk::Display::get_default();
		Gtk::StyleProvider::add_provider_for_display(display, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		auto theme = Gtk::IconTheme::get_for_display(display);
		theme->add_resource_path("/icons");
		auto window = std::make_shared<mvp::StartupWindow>(mvp::ApplicationContext::create(appController->getApp()));

		auto model = std::make_shared<mvp::ModelStartup>();
		auto presenter = std::make_shared<mvp::PresenterStartup>(window, model);
		appController->run(presenter);
		//addWindow(presenter);
		std::thread([]() { runImgui(); }).detach();


		//application->add_window(*window->getCreateProjectWindow()->window);
	}

	static void addWindow(const std::shared_ptr<mvp::IPresenter> &pWindow) { windows.emplace(pWindow); }

	static void removeWindow(const std::shared_ptr<mvp::IPresenter> &pWindow) { windows.erase(pWindow); }
};

class EditorWindow : public sdk::Window {
	std::shared_ptr<mvp::IApplicationController> appController;

public:
	explicit EditorWindow(const std::shared_ptr<mvp::IApplicationController> &pAppController)
		: appController(pAppController) {
		glfwWindowHint(GLFW_SAMPLES, 4);
	}

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
		if (auto msg = project->openDatabase()) { return msg; }
		project->initProject();

		if (auto msg = project->saveProject()) { return msg; }

		appController->setSceneUi(mainUi);
		Window::init();
		/*sdk::ReportMessagePtr msg;
		auto viewMain = mvp::MainWindow::create(mvp::UiWindowContext::create(mainUi), msg);
		if (msg) { return msg; }
		auto modelMain = std::make_shared<mvp::ModelMain>();
		modelMain->setProject(project);
		appController->run(std::make_shared<mvp::PresenterMain>(viewMain, modelMain));*/

		return nullptr;
	}
};

int GameEngine::run(const int pArgc, char* pArgv[]) {
#ifndef USE_OLD_UI
	Gio::init();
	Globals::init();
	auto appController = std::make_shared<mvp::ApplicationController>();
	auto application = sdk::Application::create();

	application->initEngine();
	auto window = std::make_shared<EditorWindow>(appController);
	application->registerWindow(window);


	auto result = application->runMainLoop(pArgc, pArgv);
	appController.reset();
	return result;

#else

	return GameEngineImpl::run(pArgc, pArgv);
#endif
}

void GameEngine::addWindow(std::shared_ptr<mvp::IPresenter> pWindow) { GameEngineImpl::addWindow(std::move(pWindow)); }

void GameEngine::removeWindow(std::shared_ptr<mvp::IPresenter> pWindow) {
	GameEngineImpl::removeWindow(std::move(pWindow));
}

} // namespace mer::editor::ui