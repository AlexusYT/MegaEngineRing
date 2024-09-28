//
// Created by alexus on 11.09.24.
//

#include "CustomSignalListItemFactory.h"

#include <gtkmm/treeexpander.h>
#include <gtkmm/treelistrow.h>

#include "TreeElementBase.h"

namespace mer::editor::ui {
CustomSignalListItemFactory::CustomSignalListItemFactory() {
	signal_bind().connect([this](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pItem->get_item());
		if (!row) return;
		const auto data = std::dynamic_pointer_cast<TreeElementBase>(row->get_item());
		if (!data) return;

		const auto widget = pItem->get_child();
		widget->set_data("contextSelectedElement", data.get());
		auto expander = dynamic_cast<Gtk::TreeExpander*>(widget);
		if (!expander) return;
		data->setOnShowExpanderSlot([expander](const bool pShow) { expander->set_hide_expander(!pShow); });
	});
	signal_unbind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto widget = pItem->get_child();
		widget->remove_data("contextSelectedElement");
	});
}

std::shared_ptr<CustomSignalListItemFactory> CustomSignalListItemFactory::create() {
	return make_refptr_for_instance(new CustomSignalListItemFactory());
}
} // namespace mer::editor::ui
