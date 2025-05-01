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
// Created by alexus on 23.09.24.
//

#include "ProjectExplorerElement.h"

namespace mer::editor::mvp {
ProjectExplorerElement::ProjectExplorerElement(const std::filesystem::path &pPath,
											   const std::filesystem::path &pRootPath, const bool pDirectory)
	: directory(pDirectory), path(pPath), rootPath(pRootPath) {

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
	/*fileMonitor = Gio::File::create_for_path(path.string())->monitor_directory();

	fileMonitor->signal_changed().connect([this, pRootPath](const std::shared_ptr<Gio::File> &pFile,
															const std::shared_ptr<Gio::File> & pOtherFile,
															const Gio::FileMonitor::Event &pEventType) {
		const auto file = std::filesystem::path(pFile->get_path());
		if (pEventType == Gio::FileMonitor::Event::DELETED) {
			for (uint32_t i = 0, maxI = children.size(); i < maxI; ++i) {
				if (const auto childSelf = children.at(i); childSelf->getPath() == file) {
					removeChild(childSelf);
					return;
				}
			}
		}
		if (pEventType == Gio::FileMonitor::Event::CREATED) { addChild(create(file, pRootPath, is_directory(file))); }
	});*/
}

void ProjectExplorerElement::addChild(const std::shared_ptr<ProjectExplorerElement> &pChild) {
	children.emplace_back(pChild);
	pChild->parent = this;
}

void ProjectExplorerElement::removeChild(const std::shared_ptr<ProjectExplorerElement> & /*pChild*/) const {
	/*
	auto result = children.find(pChild);
	if (!result.first) return;
	children->remove(result.second);*/
}

void ProjectExplorerElement::sort() const {
	/*children.sort([](const std::shared_ptr<const ProjectExplorerElement> &pFirst,
					 const std::shared_ptr<const ProjectExplorerElement> &pSecond) {
		if (pFirst->isDirectory() && !pSecond->directory) return -1;
		if (!pFirst->directory && pSecond->isDirectory()) return 1;
		return pFirst->getName().lowercase().compare(pSecond->getName().lowercase());
	});*/
}

} // namespace mer::editor::mvp