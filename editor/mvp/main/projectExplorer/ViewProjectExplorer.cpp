// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 19.01.24.
//

#include "ViewProjectExplorer.h"

#include <project/Project.h>

#include "entries/ProjectExplorerElement.h"
#include "mvp/contexts/IWidgetContext.h"
#include "ui/customWidgets/CustomSignalListItemFactory.h"
#include "ui/customWidgets/CustomTreeView.h"
#include "ui/utils/UiUtils.h"

namespace mer::editor::mvp {
ViewProjectExplorer::ViewProjectExplorer(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {

	treeView = Gtk::make_managed<ui::CustomTreeView>();
	treeView->set_halign(Gtk::Align::START);
	mainScrolledWindow.set_child(*treeView);
	mainScrolledWindow.set_has_frame(true);
	ViewProjectExplorer::setSelectOnDoubleClick(true);
	addNameColumn();
	addTypeColumn();
	ViewProjectExplorer::setSelectionChangedSlot([this](const ProjectExplorerElement* pElement) {
		auto str = pElement->getPath().string();
		treeView->activate_action("file.open", Glib::Variant<Glib::ustring>::create(str));
	});
}

void ViewProjectExplorer::setSlotCreateModel(
	const sigc::slot<std::shared_ptr<Gio::ListModel>(const std::shared_ptr<Glib::ObjectBase> &)> &pSlot) {
	treeView->setSlotCreateModel(pSlot);
}

void ViewProjectExplorer::setSelectionChangedSlot(const sigc::slot<void(ProjectExplorerElement* pElement)> &pSlot) {

	treeView->setSlotSelectionChanged([this, pSlot](Glib::ObjectBase* pObjectBase) {
		const auto element = dynamic_cast<ProjectExplorerElement*>(pObjectBase);
		if (!element) return;
		pSlot(element);
	});
}

void ViewProjectExplorer::setSelectOnDoubleClick(const bool pSelectOnDoubleClick) {
	treeView->setSelectOnDoubleClick(pSelectOnDoubleClick);
}

void ViewProjectExplorer::openView() { context->addWidget(&mainScrolledWindow); }

void ViewProjectExplorer::closeView() { context->removeWidget(); }

void ViewProjectExplorer::addNameColumn() const {

	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* expander = Gtk::make_managed<Gtk::TreeExpander>();
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		expander->set_child(*label);
		pListItem->set_child(*expander);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;

		const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(row->get_item());
		if (!col) return;
		auto* const expander = dynamic_cast<Gtk::TreeExpander*>(pListItem->get_child());
		if (!expander) return;
		expander->set_list_row(row);
		auto* label = dynamic_cast<Gtk::Label*>(expander->get_child());
		if (!label) return;
		label->set_text(col->getName());
	});

	const auto column = Gtk::ColumnViewColumn::create("Name", factory);
	column->set_resizable(true);
	treeView->append_column(column);
}

void ViewProjectExplorer::addTypeColumn() const {

	const auto factory = ui::CustomSignalListItemFactory::create();
	factory->signal_setup().connect([this](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		auto* label = Gtk::make_managed<Gtk::Label>();
		label->set_halign(Gtk::Align::END);
		label->set_margin(3);
		pListItem->set_child(*label);
	});
	factory->signal_bind().connect([](const Glib::RefPtr<Gtk::ListItem> &pListItem) {
		const auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(pListItem->get_item());
		if (!row) return;
		const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(row->get_item());
		if (!col) return;
		auto* label = dynamic_cast<Gtk::Label*>(pListItem->get_child());
		if (!label) return;
		switch (col->getType()) {
			case ExplorerElementType::DIRECTORY: label->set_text("Folder"); break;
			case ExplorerElementType::SCENE: label->set_text("Scene"); break;
			case ExplorerElementType::RESOURCE_MODEL: label->set_text("Model"); break;
			case ExplorerElementType::RESOURCE_TEXTURE: label->set_text("Texture"); break;
			case ExplorerElementType::RESOURCE_MATERIAL: label->set_text("Material"); break;
			default:; label->set_text("File");
		}
	});

	const auto column = Gtk::ColumnViewColumn::create("Type", factory);
	column->set_resizable(true);
	treeView->append_column(column);
}

void ViewProjectExplorer::onPathChanged(const std::filesystem::path &pPath) const {
	auto menu = Gio::Menu::create();
	const auto variant = Glib::Variant<Glib::ustring>::create(pPath.string());
	const auto menuNewResource = Gio::Menu::create();
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Model", "new.resource.model", variant));
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Material", "new.resource.material", variant));
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Texture", "new.resource.texture", variant));
	const auto menuNew = Gio::Menu::create();
	menuNew->append_submenu("Resource", menuNewResource);
	menuNew->append_item(ui::UiUtils::createMenuItem("Folder", "new.folder", variant));
	menuNew->append_item(ui::UiUtils::createMenuItem("Scene", "new.scene", variant));
	menuNew->append_item(ui::UiUtils::createMenuItem("Script", "new.script", variant));
	menu->append_submenu("New", menuNew);
	treeView->setMenu(menu);
}

void ViewProjectExplorer::selectByUri(const std::filesystem::path &pPath) {
	auto selection = std::dynamic_pointer_cast<Gtk::SingleSelection>(treeView->get_model());
	if (!selection) return;
	for (uint32_t i = 0, maxI = selection->get_n_items(); i < maxI; ++i) {
		auto row = selection->get_typed_object<Gtk::TreeListRow>(i);
		if (!row) return;

		auto pos = getPositionByUri(pPath, row);
		if (!pos) continue;
		selection->select_item(pos.value(), true);
	}
}

std::optional<uint32_t> ViewProjectExplorer::getPositionByUri(const std::filesystem::path &pPath,
															  const std::shared_ptr<Gtk::TreeListRow> &pRow) {

	auto item = std::dynamic_pointer_cast<ProjectExplorerElement>(pRow->get_item());
	if (!item) return {};
	auto relativePath = item->getRelativePath();
	if (relativePath == pPath) return pRow->get_position();

	pRow->set_expanded(true);
	auto children = pRow->get_children();

	for (uint32_t i = 0, maxI = children->get_n_items(); i < maxI; ++i) {
		auto row = pRow->get_child_row(i);
		if (!row) return {};
		return getPositionByUri(pPath, row);
	}
	pRow->set_expanded(false);
	return {};
}
} // namespace mer::editor::mvp