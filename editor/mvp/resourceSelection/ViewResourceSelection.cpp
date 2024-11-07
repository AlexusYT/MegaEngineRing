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
// Created by alexus on 30.09.24.
//

#include "ViewResourceSelection.h"

#include <memory>

#include "Globals.h"
#include "IPresenterResourceSelection.h"
#include "mvp/contexts/IWidgetContext.h"
#include "mvp/contexts/ViewportContext.h"
#include "mvp/main/projectExplorer/IPresenterProjectExplorer.h"
#include "mvp/main/projectExplorer/ViewProjectExplorer.h"
#include "ui/customWidgets/CustomTreeView.h"
#include "ui/utils/ErrorDialog.h"

namespace mer::editor::mvp {
class SelectionObject : public Glib::Object {
	std::string name;

	SelectionObject(const std::string &pName) : name(pName) {}

public:
	static std::shared_ptr<SelectionObject> create(const std::string &pName) {
		return Glib::make_refptr_for_instance(new SelectionObject(pName));
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }
};

ViewResourceSelection::ViewResourceSelection(const std::shared_ptr<IWidgetContext> &pContext,
											 const std::shared_ptr<Gtk::Builder> &pBuilder)
	: context(pContext), builder(pBuilder) {

	Gtk::Viewport* objectsViewport = builder->get_widget<Gtk::Viewport>("objectsViewport");
	explorer = std::make_shared<ViewProjectExplorer>(ViewportContext::create(objectsViewport));
	explorer->setSelectionChangedSlot(
		[this](ProjectExplorerElement* pElement) { presenter->onElementSelectionChanged(pElement); });
	explorer->setSelectOnDoubleClick(false);
	listView = builder->get_widget<Gtk::ListView>("objectsListView");

	//mainPaned->set_start_child(*resourceTreeView);
	auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		auto label = Gtk::make_managed<Gtk::Label>();
		label->set_margin(3);
		label->set_halign(Gtk::Align::START);
		pItem->set_child(*label);
	});
	factory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		auto data = std::dynamic_pointer_cast<SelectionObject>(pItem->get_item());
		if (!data) return;
		auto btn = dynamic_cast<Gtk::Label*>(pItem->get_child());
		if (!btn) return;
		btn->set_label(data->getName());
	});

	listView->set_factory(factory);

	builder->get_widget<Gtk::Button>("selectBtn")->signal_clicked().connect([this] {
		if (presenter) presenter->selectClicked();
	});

	builder->get_widget<Gtk::Button>("unsetBtn")->signal_clicked().connect([this] {
		if (presenter) presenter->unsetClicked();
	});

	builder->get_widget<Gtk::Button>("cancelBtn")->signal_clicked().connect([this] {
		if (presenter) presenter->onCancelClicked();
	});
	signal_close_request().connect(
		[this] {
			if (presenter) presenter->onCancelClicked();
			return true;
		},
		false);
	set_child(*builder->get_widget<Gtk::Widget>("root"));
	set_default_size(400, 300);
}

std::shared_ptr<ViewResourceSelection> ViewResourceSelection::create(const std::shared_ptr<IWidgetContext> &pContext,
																	 sdk::utils::ReportMessagePtr &pMsg) {
	std::shared_ptr<Gtk::Builder> builder;
	try {
		builder = Gtk::Builder::create_from_resource("/ui/ResourceSelectionWindow.ui", "root");
	} catch (...) {
		pMsg = sdk::utils::ReportMessage::create();
		pMsg->setTitle("Failed to load ui file");
		pMsg->setMessage("Error while loading resource selection window");
		return nullptr;
	}
	return std::shared_ptr<ViewResourceSelection>(new ViewResourceSelection(pContext, builder));
}

void ViewResourceSelection::openView() { context->addWidget(this); }

void ViewResourceSelection::closeView() {
	context->removeWidget();
	explorer->getPresenter()->removeView(explorer);
}

void ViewResourceSelection::displayError(const sdk::utils::ReportMessagePtr &pError) {
	ErrorDialog::showErrorDialog(this, pError);
}

void ViewResourceSelection::setSelectButtonSensitivity(const bool pState) const {
	builder->get_widget<Gtk::Button>("selectBtn")->set_sensitive(pState);
}

void ViewResourceSelection::displayMessage(const std::string &pMessage) {
	const auto label = builder->get_widget<Gtk::Label>("messageLabel");
	label->set_visible(!pMessage.empty());
	label->set_text(pMessage);
}

void ViewResourceSelection::displayObjects(const std::vector<std::string> &pObjects) {
	if (pObjects.empty()) {
		listView->set_model(nullptr);
		return;
	}
	auto objectsList = Gio::ListStore<SelectionObject>::create();

	for (auto object: pObjects) {
		auto modelObject = SelectionObject::create(object);
		objectsList->append(modelObject);
	}
	objectsList->sort(
		[](const Glib::RefPtr<const SelectionObject> &pFirst, const Glib::RefPtr<const SelectionObject> &pSecond) {
			return pFirst->getName().compare(pSecond->getName());
		});
	auto selectionModel = Gtk::SingleSelection::create(objectsList);
	auto selectedItemProp = selectionModel->property_selected_item();
	selectedItemProp.signal_changed().connect([selectedItemProp, this] {
		auto item = selectedItemProp.get_value();
		if (auto object = std::dynamic_pointer_cast<SelectionObject>(item)) {
			if (presenter) presenter->onObjectSelectionChanged(object->getName());
		}
	});
	selectionModel->set_can_unselect();
	selectionModel->set_autoselect(false);
	selectionModel->set_selected(GTK_INVALID_LIST_POSITION);
	listView->set_model(selectionModel);
}

void ViewResourceSelection::selectResourceByUri(const std::filesystem::path &pPath) { explorer->selectByUri(pPath); }

void ViewResourceSelection::selectObject(const std::string &pObjectToSelect) {

	auto selectionModel = std::dynamic_pointer_cast<Gtk::SingleSelection>(listView->get_model());
	if (!selectionModel) return;

	for (uint32_t i = 0, maxI = selectionModel->get_n_items(); i < maxI; ++i) {
		auto object = selectionModel->get_typed_object<SelectionObject>(i);
		if (!object) continue;
		if (object->getName() != pObjectToSelect) continue;
		selectionModel->set_selected(i);
	}
}

void ViewResourceSelection::setVisibleNoObjectsMessage(const bool pState) {

	builder->get_widget<Gtk::Label>("noObjectsLabel")->set_visible(pState);
}

std::string ViewResourceSelection::getSelectedObject() const {
	auto selectionModel = std::dynamic_pointer_cast<Gtk::SingleSelection>(listView->get_model());
	if (!selectionModel) return "";

	if (auto object = std::dynamic_pointer_cast<SelectionObject>(selectionModel->get_selected_item())) {
		return object->getName();
	}
	return "";
}
} // namespace mer::editor::mvp