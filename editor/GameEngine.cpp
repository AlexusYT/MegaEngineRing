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

namespace mer::editor::ui {

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
		application->signal_activate().connect(&GameEngineImpl::startup);
		application->register_application();
		auto result = application->run(pArgc, pArgv);
		windows.clear();
		return result;
	}

	static void signals(int pSig) {
		auto msg = sdk::utils::ReportMessage::create();
		msg->setTitle("Signal handled");
		msg->setMessage("Aborting...");
		msg->addInfoLine("Signal: {}", pSig);
		sdk::utils::Logger::error(msg);
		std::exit(-1);
	}

	static void startup() {
		using namespace mer::sdk::utils;
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

		if (!exists(Globals::getResourcesPath())) {
			Logger::warn("There is no resources at: {}", Globals::getResourcesPath().parent_path().string());
			auto path = std::filesystem::current_path();
			Logger::info("Checking resources at the executable dir: {}", path.string());
			if (!exists(path / "Resources")) {
				Logger::error("No resources directory found. Exiting...");
				return;
			}
			Globals::setResourcesPath(path / "Resources");
			Logger::info("Using resources from the executable");
		}


		auto sdk = Globals::getSdkPath();
		std::string version = Globals::getSdkVersion();
		if (version.empty()) {
			//TODO add regex check
			for (auto entry: std::filesystem::directory_iterator(sdk)) { version = entry.path().filename(); }
			Logger::warn("SDK version not specified. Using {}", version);
			Globals::setSdkVersion(version);
		}
		sdk /= version;
		if (!exists(sdk)) {
			const auto msg = ReportMessage::create();
			msg->setTitle("Failed to detect sdk");
			msg->setMessage("No sdk directory");
			msg->addInfoLine("Path to sdk: {}", sdk.string());
			Logger::error(msg);
			return;
		}

		if (!exists(sdk / "lib/MegaEngineSDK.a")) {
			const auto msg = ReportMessage::create();
			msg->setTitle("Failed to detect sdk");
			msg->setMessage("File lib/MegaEngineSDK.a not found in sdk directory");
			msg->addInfoLine("SDK directory: {}", sdk.string());
			Logger::error(msg);
			return;
		}
		Logger::info("Detected SDK: {}", sdk.string());
		Globals::setSdk(sdk);

		auto builder = Gtk::Builder::create();
		try {
			builder->add_from_file(Globals::getResourcesPath() / "CreateOpenProject.ui");
		} catch (...) {
			auto msg = ReportMessage::create();
			msg->setTitle("Failed to init window");
			msg->setMessage("Error while loading UI from file");
			Logger::error(msg);
			return;
		}


		const std::shared_ptr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
		cssProvider->signal_parsing_error().connect(
			[](const Glib::RefPtr<const Gtk::CssSection> & /*section*/, const Glib::Error &pError) {
				//section.
				Logger::error(pError.what());
			},
			true);
		try {
			cssProvider->load_from_path(Globals::getResourcesPath() / "style.css");
		} catch (Glib::Error &e) { Logger::error("Failed to load style {}", e.what()); }
		Gtk::StyleContext::add_provider_for_display(Gdk::Display::get_default(), cssProvider,
													GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

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

} // namespace mer::editor::ui