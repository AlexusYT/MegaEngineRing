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

#include "ui/utils/UiUtils.h"

namespace mer::editor::mvp {
ProjectExplorerElement::ProjectExplorerElement(const std::filesystem::path &pPath,
											   const std::filesystem::path &pRootPath, const bool pDirectory)
	: TreeElementBase(Gio::ListStore<ProjectExplorerElement>::create()), directory(pDirectory), path(pPath),
	  rootPath(pRootPath) {

	auto ext = path.extension();
	if (directory) type = ExplorerElementType::DIRECTORY;
	else if (ext == ".enscene")
		type = ExplorerElementType::SCENE;
	else if (ext == ".enmodel")
		type = ExplorerElementType::RESOURCE_MODEL;
	else if (ext == ".entex")
		type = ExplorerElementType::RESOURCE_TEXTURE;
	else if (ext == ".enmat")
		type = ExplorerElementType::RESOURCE_MATERIAL;
	fileMonitor = Gio::File::create_for_path(path.string())->monitor_directory();

	fileMonitor->signal_changed().connect([this, pRootPath](const std::shared_ptr<Gio::File> &pFile,
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
		if (pEventType == Gio::FileMonitor::Event::CREATED) { addChild(create(file, pRootPath, is_directory(file))); }
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
	const auto menuNewResource = Gio::Menu::create();
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Model", "new.resource.model", variant));
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Material", "new.resource.material", variant));
	menuNewResource->append_item(ui::UiUtils::createMenuItem("Texture", "new.resource.texture", variant));
	const auto menuNew = Gio::Menu::create();
	menuNew->append_submenu("Resource", menuNewResource);
	menuNew->append_item(ui::UiUtils::createMenuItem("Folder", "new.folder", variant));
	menuNew->append_item(ui::UiUtils::createMenuItem("Scene", "new.scene", variant));
	menuNew->append_item(ui::UiUtils::createMenuItem("Script", "new.script", variant));
	const auto menuManage = Gio::Menu::create();
	menuManage->append_submenu("New", menuNew);
	menuManage->append_item(ui::UiUtils::createMenuItem("Rename", "file.manage.rename", variant));
	menuManage->append_item(ui::UiUtils::createMenuItem("Delete", "file.manage.delete", variant));
	menu->append_section(menuManage);

	const auto menuMisc = Gio::Menu::create();
	menuMisc->append_item(ui::UiUtils::createMenuItem("Show in Files", "file.manage.showInFiles", variant));
	menu->append_section(menuMisc);
	return menu;
}

} // namespace mer::editor::mvp