//
// Created by alexus on 30.10.2021.
//
#include "MainWindow.h"

#include <glibmm/fileutils.h>
#include <glibmm/markup.h>
#include <gtkmm/applicationwindow.h>
#include <iostream>
#include <stacktrace>

#include "graphic/viewport/OpenGLRender.h"

MainWindow::MainWindow() {}

MainWindow::~MainWindow() {}

bool MainWindow::init() {
	// Load the GtkBuilder file and instantiate its widgets:
	builder = Gtk::Builder::create();
	try {
		builder->add_from_file("Resources/base.ui");
	} catch (const Glib::FileError &ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return false;
	} catch (const Glib::MarkupError &ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return false;
	} catch (const Gtk::BuilderError &ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return false;
	}

	// Get the GtkBuilder-instantiated dialog:
	mainWindow = builder->get_widget<Gtk::Window>("MainWindow");
	if (!mainWindow) {
		std::cerr << "Could not get the dialog" << std::endl;
		return false;
	}

	auto* area = builder->get_widget<Gtk::GLArea>("OpenGLRenderer");
	render = OpenGLRender::create(area);
	render->init();
	/*
	// Get the GtkBuilder-instantiated button, and connect a signal handler:
	auto pButton = refBuilder->get_widget<Gtk::Button>("button");
	if (pButton) pButton->signal_clicked().connect([]() { on_button_clicked(); });
	*/


	mainWindow->set_visible(true);
	return true;
}
