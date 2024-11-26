//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 17.11.24.
//

#include "SourceSelectionResource.h"

#include "EngineSDK/main/resources/IResource.h"
#include "EngineSDK/main/resources/IResourceLoadExecutor.h"
#include "EngineSDK/main/resources/LoadedResources.h"
#include "ui/customWidgets/resourceSelector/ResourceSelectorWidget.h"

namespace mer::editor::ui {
const std::string &ResourceElement::getName() const { return resource->getResourceName(); }

SourceSelectionResource::SourceSelectionResource(sdk::main::IResourceLoadExecutor* pLoader) : loader(pLoader) {}

Gtk::Widget* SourceSelectionResource::getWidget() {
	scroller = Gtk::make_managed<Gtk::ScrolledWindow>();
	scroller->signal_map().connect([this] {});
	scroller->set_size_request(205, 200);
	scroller->get_hadjustment()->set_page_size(205);
	scroller->get_vadjustment()->set_page_size(200);
	listView = Gtk::make_managed<Gtk::ListView>();
	listView->signal_realize().connect([this] {
		const auto selection = getParent()->getSelection();
		if (!selection) return;
		onResultSet(selection);
	});

	scroller->set_child(*listView);
	listView->set_single_click_activate(false);
	listView->signal_activate().connect([this](const uint32_t pIndex) {
		const auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(listView->get_model());
		const auto row = selection->get_typed_object<Gtk::TreeListRow>(pIndex);
		if (!row) return;
		const auto element = std::dynamic_pointer_cast<SelectionElement>(row->get_item());
		if (!element) return;
		if (!element->selectable()) {
			row->set_expanded(!row->get_expanded());
			return;
		}
		onElementSelected(element);
	});
	const auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto tree = Gtk::make_managed<Gtk::TreeExpander>();
		const auto label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::START);
		tree->set_child(*label);
		pItem->set_child(*tree);
	});
	factory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pItem->get_item());
		if (!row) return;
		const auto element = std::dynamic_pointer_cast<SelectionElement>(row->get_item());
		if (!element) return;
		pItem->set_selectable(element->selectable());

		const auto tree = dynamic_cast<Gtk::TreeExpander*>(pItem->get_child());
		if (!tree) return;
		tree->set_list_row(row);
		const auto label = dynamic_cast<Gtk::Label*>(tree->get_child());
		if (!label) return;
		label->set_text(element->getName());
	});
	listView->set_factory(factory);
	resources = Gio::ListStore<SelectionElement>::create();

	auto createModel = [](const Glib::RefPtr<Glib::ObjectBase> &pElement) -> std::shared_ptr<Gio::ListModel> {
		const auto data = std::dynamic_pointer_cast<SelectionElement>(pElement);
		if (!data) return nullptr;
		return data->getChildren();
	};
	const auto singleSelection = Gtk::SingleSelection::create(Gtk::TreeListModel::create(resources, createModel));
	singleSelection->set_can_unselect(true);
	singleSelection->set_autoselect(false);
	singleSelection->set_selected(GTK_INVALID_LIST_POSITION);
	listView->set_model(singleSelection);
	return scroller;
}

void SourceSelectionResource::onShow() {
	resources->remove_all();
	for (auto resource: loader->getResources()->getResources() | std::views::values) {
		if (resource->getResourceType() != getResourceType()) continue;
		resources->append(createElementFromResource(resource));
	}
	resources->sort(
		[](const std::shared_ptr<const SelectionElement> &pA, const std::shared_ptr<const SelectionElement> &pB) {
			return pA->getName().compare(pB->getName());
		});
}

std::shared_ptr<Gtk::TreeListRow> SourceSelectionResource::findRow(const sdk::main::IResource* pResource) const {
	auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(listView->get_model());

	for (auto i = 0u, maxI = selection->get_n_items(); i < maxI; ++i) {
		std::shared_ptr<Gtk::TreeListRow> row = selection->get_typed_object<Gtk::TreeListRow>(i);
		if (!row) continue;
		const auto element = std::dynamic_pointer_cast<ResourceElement>(row->get_item());
		if (!element) continue;
		if (element->getResource().get() == pResource) return row;
	}
	return nullptr;
}

void SourceSelectionResource::scrollToRow(const std::shared_ptr<Gtk::TreeListRow> &pRow) const {
	listView->scroll_to(pRow->get_position(), Gtk::ListScrollFlags::SELECT);
}
} // namespace mer::editor::ui