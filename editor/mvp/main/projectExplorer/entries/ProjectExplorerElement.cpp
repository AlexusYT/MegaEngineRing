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
// Created by alexus on 23.09.24.
//

#include "ProjectExplorerElement.h"

namespace mer::editor::mvp {
ProjectExplorerElement::ProjectExplorerElement(const std::filesystem::path &pPath, const bool pDirectory)
	: TreeElementBase(Gio::ListStore<ProjectExplorerElement>::create()), directory(pDirectory), path(pPath) {

	fileMonitor = Gio::File::create_for_path(path)->monitor_directory();

	fileMonitor->signal_changed().connect([this](const std::shared_ptr<Gio::File> &pFile,
												 const std::shared_ptr<Gio::File> & /*pOtherFile*/,
												 const Gio::FileMonitor::Event &pEventType) {
		const auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ProjectExplorerElement>>(getChildren());
		const auto file = std::filesystem::path(pFile->get_path());
		if (pEventType == Gio::FileMonitor::Event::DELETED) {
			for (uint32_t i = 0, maxI = childrenSelf->get_n_items(); i < maxI; ++i) {
				if (const auto childSelf = childrenSelf->get_item(i); childSelf->getPath() == file) {
					removeChild(childSelf);
					return;
				}
			}
		}
		if (pEventType == Gio::FileMonitor::Event::CREATED) { addChild(create(file, is_directory(file))); }
	});
}

void ProjectExplorerElement::addChild(const std::shared_ptr<ProjectExplorerElement> &pChild) {
	const auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ProjectExplorerElement>>(getChildren());
	childrenSelf->insert_sorted(pChild, [](const Glib::RefPtr<const ProjectExplorerElement> &pFirst,
										   const Glib::RefPtr<const ProjectExplorerElement> &pSecond) {
		if (pFirst->isDirectory() && !pSecond->directory) return -1;
		if (!pFirst->directory && pSecond->isDirectory()) return 1;
		return pFirst->getName().lowercase().compare(pSecond->getName().lowercase());
	});
	pChild->parent = this;
	notifyChanged();
}

void ProjectExplorerElement::removeChild(const std::shared_ptr<ProjectExplorerElement> &pChild) const {
	const auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ProjectExplorerElement>>(getChildren());
	auto result = childrenSelf->find(pChild);
	if (!result.first) return;
	childrenSelf->remove(result.second);
	notifyChanged();
}

void ProjectExplorerElement::sort() const {
	const auto childrenSelf = std::dynamic_pointer_cast<Gio::ListStore<ProjectExplorerElement>>(getChildren());
	childrenSelf->sort([](const Glib::RefPtr<const ProjectExplorerElement> &pFirst,
						  const Glib::RefPtr<const ProjectExplorerElement> &pSecond) {
		if (pFirst->isDirectory() && !pSecond->directory) return -1;
		if (!pFirst->directory && pSecond->isDirectory()) return 1;
		return pFirst->getName().lowercase().compare(pSecond->getName().lowercase());
	});
}

std::shared_ptr<Gio::MenuModel> ProjectExplorerElement::getMenu() {

	auto menu = Gio::Menu::create();
	const auto variant = Glib::Variant<Glib::ustring>::create(path.string());
	const auto menuNew = Gio::Menu::create();
	menuNew->append_item(createItem("Scene", "file.manage.new.scene", variant));
	menuNew->append_item(createItem("Script", "file.manage.new.script", variant));
	const auto menuManage = Gio::Menu::create();
	menuManage->append_item(createItem("New folder", "file.manage.new.folder", variant));
	menuManage->append_submenu("New", menuNew);
	menuManage->append_item(createItem("Rename", "file.manage.rename", variant));
	menuManage->append_item(createItem("Delete", "file.manage.delete", variant));
	menu->append_section(menuManage);

	const auto menuMisc = Gio::Menu::create();
	menuMisc->append_item(createItem("Show in Files", "file.manage.showInFiles", variant));
	menu->append_section(menuMisc);
	return menu;
}

std::shared_ptr<Gio::MenuItem> ProjectExplorerElement::createItem(const std::string &pName, const std::string &pAction,
																  const Glib::Variant<Glib::ustring> &pVariant) {
	std::shared_ptr<Gio::MenuItem> item = Gio::MenuItem::create(pName, "");
	item->set_action_and_target(pAction, pVariant);
	return item;
}
} // namespace mer::editor::mvp