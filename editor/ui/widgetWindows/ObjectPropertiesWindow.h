//
// Created by alexus on 17.01.24.
//

#ifndef OBJECTPROPERTIESWINDOW_H
#define OBJECTPROPERTIESWINDOW_H

#include <ui/customWidgets/TreeWidget.h>

namespace mer::editor::ui {
class ObjectPropertiesWindow {
public:
	Gtk::Paned mainPaned;
	TreeWidget propertiesTree;
	Gtk::Label descriptionLabel;

	ObjectPropertiesWindow() {
		Gtk::ScrolledWindow propertiesScrolledWindow;
		propertiesScrolledWindow.set_child(propertiesTree);
		mainPaned.set_start_child(propertiesScrolledWindow);
		Gtk::ScrolledWindow descScrolledWindow;
		descScrolledWindow.set_child(descriptionLabel);
		mainPaned.set_start_child(descScrolledWindow);




		descriptionLabel.set_text("Описание свойства");
	}

	operator Gtk::Widget&() { return mainPaned; }
};
} // namespace mer::editor::ui


#endif //OBJECTPROPERTIESWINDOW_H
