//
// Created by alexus on 05.12.23.
//

#include "GameEngine.h"

#include <dlfcn.h>
#include <getopt.h>
#include <mvp/startup/ModelStartup.h>
#include <mvp/startup/PresenterStartup.h>
#include <unordered_set>
#include <utility>

#include "Globals.h"
#include "cmdOptions/EngineOptionGroup.h"
#include "mvp/startup/StartupWindow.h"

namespace UI_CORE {

class GameEngineImpl {
	static inline std::shared_ptr<Gtk::Application> application;
	static inline std::unordered_set<std::shared_ptr<mvp::IPresenter>> windows;


public:
	static int run(int pArgc, char* pArgv[]) {
		Globals::init();
		signal(SIGABRT, signals);
		signal(SIGSEGV, signals);


		application = Gtk::Application::create("org.gtkmm.example");
		utils::EngineOptionGroup group;

		application->add_option_group(group);

		const std::shared_ptr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
		cssProvider->signal_parsing_error().connect(
			[](const Glib::RefPtr<const Gtk::CssSection> & /*section*/, const Glib::Error &pError) {
				//section.
				engine::utils::Logger::error(pError.what());
			},
			true);
		try {
			cssProvider->load_from_path("Resources/style.css");
		} catch (Glib::Error &e) { engine::utils::Logger::error("Failed to load style {}", e.what()); }
		Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), cssProvider,
													GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

		application->signal_startup().connect(&GameEngineImpl::startup);

		application->register_application();
		auto result = application->run(pArgc, pArgv);
		windows.clear();
		return result;
	}

	static void signals(int pSig) {
		auto msg = engine::utils::ReportMessage::create();
		msg->setTitle("Signal handled");
		msg->setMessage("Aborting...");
		msg->addInfoLine("Signal: {}", pSig);
		engine::utils::Logger::error(msg);
		std::exit(-1);
	}

	static void startup() {
		using namespace n::engine::utils;
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
		auto builder = Gtk::Builder::create();
		try {
			builder->add_from_file("Resources/CreateOpenProject.ui");
		} catch (...) {
			auto msg = ReportMessage::create();
			msg->setTitle("Failed to init window");
			msg->setMessage("Error while loading UI from file");
			Logger::error(msg);
			return;
		}
		auto tmpWindow = std::shared_ptr<mvp::StartupWindow>(
			Gtk::Builder::get_widget_derived<mvp::StartupWindow>(builder, "window_createOpenProj"));
		if (!tmpWindow) {
			auto msg = ReportMessage::create();
			msg->setTitle("Failed to init window");
			msg->setMessage("Window not found");
			msg->addInfoLine("Window name: window_createOpenProj");
			Logger::error(msg);
			return;
		}
		auto model = std::make_shared<mvp::ModelStartup>();
		auto presenter = std::make_shared<mvp::PresenterStartup>(tmpWindow, model);

		application->add_window(*tmpWindow);
		addWindow(presenter);


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

} // namespace UI_CORE