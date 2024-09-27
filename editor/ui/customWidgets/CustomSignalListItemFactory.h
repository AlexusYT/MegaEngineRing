//
// Created by alexus on 11.09.24.
//

#ifndef CUSTOMSIGNALLISTITEMFACTORY_H
#define CUSTOMSIGNALLISTITEMFACTORY_H
#include <gtkmm/signallistitemfactory.h>

namespace mer::editor::ui {
class CustomSignalListItemFactory : public Gtk::SignalListItemFactory {

	CustomSignalListItemFactory();

public:
	static std::shared_ptr<CustomSignalListItemFactory> create();
};
} // namespace mer::editor::ui


#endif //CUSTOMSIGNALLISTITEMFACTORY_H
