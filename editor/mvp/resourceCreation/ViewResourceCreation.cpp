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

#include "ViewResourceCreation.h"

#include "Globals.h"
#include "IPresenterResourceCreation.h"
#include "mvp/contexts/IWidgetContext.h"
#include "ui/utils/ErrorDialog.h"

namespace mer::editor::mvp {
class ModelObject : public Glib::Object {
	std::string name;
	bool checked{};

	ModelObject(const std::string &pName, bool pChecked) : name(pName), checked(pChecked) {}

public:
	static std::shared_ptr<ModelObject> create(const std::string &pName, bool pChecked) {
		return Glib::make_refptr_for_instance(new ModelObject(pName, pChecked));
	}

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] bool isChecked() const { return checked; }

	void setChecked(bool pChecked) { checked = pChecked; }
};

ViewResourceCreation::ViewResourceCreation(const std::shared_ptr<IWidgetContext> &pContext,
										   const std::shared_ptr<Gtk::Builder> &pBuilder)
	: context(pContext), builder(pBuilder) {
	objectsListView = builder->get_widget<Gtk::ListView>("objectsListView");
	auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		auto btn = Gtk::make_managed<Gtk::CheckButton>();
		btn->set_margin(3);
		pItem->set_child(*btn);
	});
	factory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> &pItem) {
		auto data = std::dynamic_pointer_cast<ModelObject>(pItem->get_item());
		if (!data) return;
		auto btn = dynamic_cast<Gtk::CheckButton*>(pItem->get_child());
		if (!btn) return;
		btn->set_label(data->getName());
		btn->set_active(data->isChecked());
		btn->property_active().signal_changed().connect([data, btn] { data->setChecked(btn->get_active()); });
	});
	objectsListView->set_factory(factory);

	builder->get_widget<Gtk::Button>("saveBtn")->signal_clicked().connect([this] {
		if (presenter) presenter->saveClicked();
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
	auto nameEntry = builder->get_widget<Gtk::Entry>("nameEntry");
	nameEntry->signal_changed().connect([this, nameEntry] {
		if (presenter) presenter->onResourceNameChanged(nameEntry->get_text());
	});

	builder->get_widget<Gtk::Entry>("filePathEntry")
		->signal_icon_release()
		.connect([this](Gtk::Entry::IconPosition /*pPosition*/) {
			if (presenter) presenter->chooseFileClicked();
		});
	set_child(*builder->get_widget<Gtk::Widget>("root"));
}

std::shared_ptr<ViewResourceCreation> ViewResourceCreation::create(const std::shared_ptr<IWidgetContext> &pContext,
																   sdk::utils::ReportMessagePtr &pMsg) {
	std::shared_ptr<Gtk::Builder> builder;
	try {
		builder = Gtk::Builder::create_from_file(Globals::getResourcesPath() / "ResourceCreationWindow.ui", "root");
	} catch (...) {
		pMsg = sdk::utils::ReportMessage::create();
		pMsg->setTitle("Failed to load ui file");
		pMsg->setMessage("Error while loading resource creation window");
		return nullptr;
	}
	return std::shared_ptr<ViewResourceCreation>(new ViewResourceCreation(pContext, builder));
}

void ViewResourceCreation::openView() { context->addWidget(this); }

void ViewResourceCreation::closeView() { context->removeWidget(); }

void ViewResourceCreation::displayError(const sdk::utils::ReportMessagePtr &pError) {
	ErrorDialog::showErrorDialog(this, pError);
}

void ViewResourceCreation::displayChosenPath(const std::string &pPath) {
	builder->get_widget<Gtk::Entry>("filePathEntry")->set_text(pPath);
}

void ViewResourceCreation::displayResourceName(const std::string &pName) {
	builder->get_widget<Gtk::Entry>("nameEntry")->set_text(pName);
}

void ViewResourceCreation::switchTo(const std::string &pTabName) {
	builder->get_widget<Gtk::Stack>("preferencesStack")->set_visible_child(pTabName);
}

void ViewResourceCreation::setStackVisibility(const bool pState) {
	builder->get_widget<Gtk::Stack>("preferencesStack")->set_visible(pState);
}

void ViewResourceCreation::setSaveButtonSensitivity(const bool pState) const {
	builder->get_widget<Gtk::Button>("saveBtn")->set_sensitive(pState);
}

void ViewResourceCreation::displayMessage(const std::string &pMessage) {
	const auto label = builder->get_widget<Gtk::Label>("messageLabel");
	label->set_visible(!pMessage.empty());
	label->set_text(pMessage);
}

void ViewResourceCreation::displayObjects(const std::vector<std::string> &pObjects) {

	auto objectsList = Gio::ListStore<ModelObject>::create();

	for (auto object: pObjects) {
		auto modelObject = ModelObject::create(object, true);
		objectsList->append(modelObject);
	}

	auto selectionModel = Gtk::NoSelection::create(objectsList);
	objectsListView->set_model(selectionModel);
}

std::vector<std::string> ViewResourceCreation::getSelectedObjects() {
	auto selectionModel = std::dynamic_pointer_cast<Gtk::NoSelection>(objectsListView->get_model());
	if (!selectionModel) return {};
	std::vector<std::string> objects;
	for (uint32_t i = 0, maxI = selectionModel->get_n_items(); i < maxI; i++) {
		auto object = selectionModel->get_typed_object<ModelObject>(i);
		if (!object || !object->isChecked()) continue;
		objects.push_back(object->getName());
	}
	return objects;
}
} // namespace mer::editor::mvp