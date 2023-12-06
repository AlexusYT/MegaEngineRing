//
// Created by alexus on 05.12.23.
//

#include "GameEngine.h"

#include <MainWindow.h>

int GameEngine::run(const int argc, char* argv[]) {


	Glib::RefPtr<Gtk::Application> application = Gtk::Application::create("org.gtkmm.example");
	auto window = std::make_shared<MainWindow>();
	if (!window->init()) return 1;

	application->signal_activate().connect([application, window] { application->add_window(*window->mainWindow); });
	return application->run(argc, argv);
}
