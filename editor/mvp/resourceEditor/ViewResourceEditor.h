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

#ifndef VIEWRESOURCEEDITOR_H
#define VIEWRESOURCEEDITOR_H
#include "IViewResourceEditor.h"
#include "mvp/ThreadDispatcher.h"

namespace mer::sdk::main {
class IModel3DObject;
}

namespace mer::editor::mvp {

class OpenedResource : public Glib::Object {
	std::shared_ptr<sdk::main::IResource> resource{};

	explicit OpenedResource(const std::shared_ptr<sdk::main::IResource> &pResource) : resource(pResource) {}

public:
	static std::shared_ptr<OpenedResource> create(const std::shared_ptr<sdk::main::IResource> &pResource) {
		return Glib::make_refptr_for_instance(new OpenedResource(pResource));
	}

	[[nodiscard]] const std::shared_ptr<sdk::main::IResource> &getResource() const { return resource; }

	std::string getName() const { return resource->getResourceUri().string(); }

	std::string getType() const;
};

class ViewResourceEditor : public IViewResourceEditor, ThreadDispatcher {
	IPresenterResourceEditor* presenter{};
	std::shared_ptr<IWidgetContext> context;
	std::shared_ptr<Gtk::Builder> builder;
	Gtk::ListView* fileObjectsListView;
	Gtk::ListView* resourceObjectsListView;
	Gtk::Widget* widget;
	Gtk::Stack* mainStack;
	//Gtk::Box* templateStackPage;
	Gtk::Entry* resourcePathEntry;
	Gtk::Entry* resourceNameEntry;
	Gtk::Box* unsavedDialogBox;
	Gtk::ColumnView* openedColumnView;
	std::shared_ptr<Gio::ListStore<OpenedResource>> openedList;
	Gtk::MenuButton* selectHistoryMb{};
	std::shared_ptr<Gtk::SingleSelection> selectionModel;
	Gtk::Stack* preferencesStack;

	explicit ViewResourceEditor(const std::shared_ptr<IWidgetContext> &pContext,
								const std::shared_ptr<Gtk::Builder> &pBuilder);

public:
	static std::shared_ptr<ViewResourceEditor> create(const std::shared_ptr<IWidgetContext> &pContext);

	void openView() override;

	void closeView() override;

	void setPresenter(IPresenterResourceEditor* pPresenter) override;

	sigc::connection connectChooseFile(const sigc::slot<void(Gtk::Entry::IconPosition)> &pSlot) const override {
		return builder->get_widget<Gtk::Entry>("filePathEntry")->signal_icon_release().connect(pSlot);
	}

	void showFileDialog(const std::shared_ptr<Gtk::FileDialog> &pDialog,
						const sigc::slot<void(std::shared_ptr<Gio::AsyncResult> &pResult)> &pSlot) override {

		pDialog->open(*dynamic_cast<Gtk::Window*>(widget->get_root()), pSlot);
	}

	void displayError(const sdk::utils::ReportMessagePtr &pError) override;

	void displayChosenPath(const std::string &pPath) override;

	void displayResourceName(const std::string &pName) override;

	void switchTo(const std::string &pTabName) override;

	void displayMessage(const std::string &pMessage) override;

	void displayObjects(const std::vector<std::shared_ptr<sdk::main::IModel3DObject>> &pObjects,
						bool pFileObjects) override;

	void setStackVisibility(bool pState) override;

	void setSaveButtonSensitivity(bool pState) const override;

	std::vector<std::string> getSelectedObjects() override;

	void appendResource(const std::shared_ptr<sdk::main::IResource> &pNewResource) override;

	void selectResource(const std::shared_ptr<sdk::main::IResource> &pResource) override;

private:
	static void markErrored(Gtk::Widget* pWidget);

	static void unmarkErrored(Gtk::Widget* pWidget);

	void updateUnsavedDialog(const std::shared_ptr<sdk::main::IResource> &pResource) const;

	void updateResourcePath(const std::shared_ptr<sdk::main::IResource> &pResource) const;

	void setupColumnView();

	Gtk::Entry* getFilePathEntry() const;
};


} // namespace mer::editor::mvp

#endif //VIEWRESOURCEEDITOR_H
