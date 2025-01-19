//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
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

#ifndef SOURCESELECTIONRESOURCE_H
#define SOURCESELECTIONRESOURCE_H
#include "EngineSDK/main/resources/ResourceType.h"
#include "ui/customWidgets/resourceSelector/SourceSelection.h"

namespace mer::sdk::main {
class IResource;
class IResourceLoadExecutor;
} // namespace mer::sdk::main

namespace mer::editor::ui {
class SelectionElement : public Glib::Object {
public:
	virtual std::shared_ptr<Gio::ListStore<SelectionElement>> getChildren() = 0;

	virtual const std::string &getName() const = 0;

	virtual bool selectable() = 0;
};

class ResourceElement : public SelectionElement {
	std::shared_ptr<sdk::main::IResource> resource;

protected:
	explicit ResourceElement(const std::shared_ptr<sdk::main::IResource> &pResource) : resource(pResource) {}

public:
	static std::shared_ptr<ResourceElement> create(const std::shared_ptr<sdk::main::IResource> &pResource) {
		return Glib::make_refptr_for_instance(new ResourceElement(pResource));
	}

	bool selectable() override { return true; }

	const std::string &getName() const override;

	[[nodiscard]] const std::shared_ptr<sdk::main::IResource> &getResource() const { return resource; }

	template<typename T>
	[[nodiscard]] std::shared_ptr<T> getTypedResource() const {
		return std::dynamic_pointer_cast<T>(resource);
	}

	std::shared_ptr<Gio::ListStore<SelectionElement>> getChildren() override { return nullptr; }
};

class SourceSelectionResource : public SourceSelection {

	sdk::main::IResourceLoadExecutor* loader{};
	Gtk::ListView* listView{};
	Gtk::ScrolledWindow* scroller{};
	std::shared_ptr<Gio::ListStore<SelectionElement>> resources{};

protected:
	explicit SourceSelectionResource(sdk::main::IResourceLoadExecutor* pLoader);

public:
	Gtk::Widget* getWidget() override;

	void onShow() override;

	virtual void onElementSelected(const std::shared_ptr<SelectionElement> &pElement) = 0;

	virtual sdk::main::ResourceType getResourceType() = 0;

	virtual std::shared_ptr<SelectionElement> createElementFromResource(
		const std::shared_ptr<sdk::main::IResource> &pResource) = 0;

	[[nodiscard]] Gtk::ListView* getListView() const { return listView; }

protected:
	std::shared_ptr<Gtk::TreeListRow> findRow(const sdk::main::IResource* pResource) const;

	void scrollToRow(const std::shared_ptr<Gtk::TreeListRow> &pRow) const;
};

} // namespace mer::editor::ui

#endif //SOURCESELECTIONRESOURCE_H
