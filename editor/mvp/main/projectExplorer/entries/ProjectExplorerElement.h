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

#ifndef PROJECTEXPLORERELEMENT_H
#define PROJECTEXPLORERELEMENT_H
#include "ui/customWidgets/TreeElementBase.h"

namespace mer::editor::mvp {
class ProjectExplorerElement : public ui::TreeElementBase {
	bool directory{};
	ProjectExplorerElement* parent{};
	std::filesystem::path path{};
	std::shared_ptr<Gio::FileMonitor> fileMonitor;

public:
	ProjectExplorerElement(const std::filesystem::path &pPath, bool pDirectory);

	static std::shared_ptr<ProjectExplorerElement> create(const std::filesystem::path &pPath, const bool pDirectory) {
		return Glib::make_refptr_for_instance(new ProjectExplorerElement(pPath, pDirectory));
	}

	void addChild(const std::shared_ptr<ProjectExplorerElement> &pChild);

	void removeChild(const std::shared_ptr<ProjectExplorerElement> &pChild) const;

	void sort() const;

	std::shared_ptr<Gio::MenuModel> getMenu() override;

	[[nodiscard]] bool isDirectory() const { return directory; }

	[[nodiscard]] bool isScene() const { return path.extension() == ".enscene"; }

	[[nodiscard]] ProjectExplorerElement* getParent() const { return parent; }

	[[nodiscard]] Glib::ustring getName() const { return path.stem().string(); }

	[[nodiscard]] const std::filesystem::path &getPath() const { return path; }

	void setPath(const std::filesystem::path &pPath) { path = pPath; }

private:
	static std::shared_ptr<Gio::MenuItem> createItem(const std::string &pName, const std::string &pAction,
													 const Glib::Variant<Glib::ustring> &pVariant);
};
} // namespace mer::editor::mvp


#endif //PROJECTEXPLORERELEMENT_H
