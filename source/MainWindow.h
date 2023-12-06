//
// Created by alexus on 30.10.2021.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "graphic/viewport/OpenGLRenderFwd.h"

class MainWindow {
public:
	bool reloadUi = false;
	Gtk::Window* mainWindow = nullptr;
	Glib::RefPtr<Gtk::Builder> builder;
	OpenGLRenderUPtr render;
	/**
	 * MainWindow constructor. Starts background threads LauncherUpdateThread and ServersUpdateThread
	 *
	 * @param args program args from main(argc, argv)
	 */
	explicit MainWindow();
	~MainWindow();


	bool init();
};


#endif //GAMEENGINE_H
