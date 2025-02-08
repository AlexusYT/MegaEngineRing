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
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <thread>

#include "Globals.h"
#include "cmdOptions/EngineOptionGroup.h"
#include "examples/libs/glfw/include/GLFW/glfw3.h"
#include "mvp/ApplicationController.h"
#include "mvp/IApplicationController.h"
#include "mvp/contexts/ApplicationContext.h"
#include "mvp/startup/StartupWindow.h"

namespace mer::editor::ui {

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

class GameEngineImpl {
	static inline std::shared_ptr<mvp::IApplicationController> appController;
	static inline std::unordered_set<std::shared_ptr<mvp::IPresenter>> windows;


public:
	static void runImgui() {
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit()) return;


		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

		// Create window with graphics context
		GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
		if (window == nullptr) return;
		glfwHideWindow(window);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		// Our state

		// Main loop
		while (!glfwWindowShouldClose(window)) {
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();
			if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
				ImGui_ImplGlfw_Sleep(10);
				continue;
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static int counter = 0;
				ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
				ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
				ImGui::Text("This is some useful text.");


				if (ImGui::Button("Button")) counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", static_cast<double>(1000.0f / io.Framerate),
							static_cast<double>(io.Framerate));
				ImGui::End();
			}

			{
				ImGui::Begin("Hello, world1!");			  // Create a window called "Hello, world!" and append into it.
				ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)

				ImGui::End();
			}
			// Rendering
			ImGui::Render();
			int displayW, displayH;
			glfwGetFramebufferSize(window, &displayW, &displayH);
			glViewport(0, 0, displayW, displayH);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
		}
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

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

int GameEngine::run(const int pArgc, char* pArgv[]) { return GameEngineImpl::run(pArgc, pArgv); }

void GameEngine::addWindow(std::shared_ptr<mvp::IPresenter> pWindow) { GameEngineImpl::addWindow(std::move(pWindow)); }

void GameEngine::removeWindow(std::shared_ptr<mvp::IPresenter> pWindow) {
	GameEngineImpl::removeWindow(std::move(pWindow));
}

} // namespace mer::editor::ui