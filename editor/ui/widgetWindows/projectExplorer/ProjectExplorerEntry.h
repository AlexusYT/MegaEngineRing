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

#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

namespace mer::editor::mvp {
class IEditorPresenter;
class IEditor;
} // namespace mer::editor::mvp

namespace mer::editor::ui {

enum class EntryKind { FILE, DIRECTORY, SCENE };

class ProjectExplorerEntry : public Glib::Object {
	std::string name;
	EntryKind kind{EntryKind::DIRECTORY};
	std::string image;
	Glib::RefPtr<Gio::ListStore<ProjectExplorerEntry>> children;
	Glib::RefPtr<Gio::Menu> contextMenu;
	bool databaseSaveRequired{};
	bool fileSaveRequired{};

protected:
	ProjectExplorerEntry(const std::string &pName) : name(pName) {}

	ProjectExplorerEntry() {}

public:
	void addChildEntry(const std::shared_ptr<ProjectExplorerEntry> &pEntry) {
		if (!children) children = Gio::ListStore<ProjectExplorerEntry>::create();
		children->append(pEntry);
	}

	void sortChildren() const {
		if (!children) return;
		children->sort([](const Glib::RefPtr<const ProjectExplorerEntry> &pItem1,
						  const Glib::RefPtr<const ProjectExplorerEntry> &pItem2) {
			if (pItem1->isDirectory() && pItem2->isFile()) return -1;
			if (pItem1->isFile() && pItem2->isDirectory()) return 1;
			return pItem1->getName().compare(pItem2->getName());
		});
	}

	bool isDirectory() const { return kind == EntryKind::DIRECTORY; }

	bool isFile() const { return kind == EntryKind::FILE; }

	bool isScene() const { return kind == EntryKind::SCENE; }

	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] EntryKind getKind() const { return kind; }

	void setKind(const EntryKind pKind) { kind = pKind; }

	[[nodiscard]] const std::string &getImage() const { return image; }

	void setImage(const std::string &pImage) { image = pImage; }

	void setEntryChildren(const Glib::RefPtr<Gio::ListStore<ProjectExplorerEntry>> &pChildren) { children = pChildren; }

	[[nodiscard]] const Glib::RefPtr<Gio::ListStore<ProjectExplorerEntry>> &getEntryChildren() const {
		return children;
	}

	void setContextMenu(const Glib::RefPtr<Gio::Menu> &pContextMenu) { contextMenu = pContextMenu; }

	[[nodiscard]] const Glib::RefPtr<Gio::Menu> &getContextMenu() const { return contextMenu; }

	[[nodiscard]] bool isDatabaseSaveRequired() const { return databaseSaveRequired; }

	void setDatabaseSaveRequired(const bool pDatabaseSaveRequired = true) {
		databaseSaveRequired = pDatabaseSaveRequired;
	}

	[[nodiscard]] bool isFileSaveRequired() const { return fileSaveRequired; }

	void setFileSaveRequired(const bool pFileSaveRequired = true) { fileSaveRequired = pFileSaveRequired; }

	sdk::utils::ReportMessagePtr loadDatabase() {
		if (auto msg = onLoadDatabase()) return msg;
		if (!children) return nullptr;
		for (uint32_t i = 0; i < children->get_n_items(); i++) {
			auto child = children->get_item(i);
			if (auto msg = child->loadDatabase()) return msg;
		}
		return nullptr;
	}

	sdk::utils::ReportMessagePtr saveDatabase() {
		if (isDatabaseSaveRequired()) {
			if (auto msg = onSaveDatabase()) return msg;
			setDatabaseSaveRequired(false);
		}
		if (!children) return nullptr;
		for (uint32_t i = 0; i < children->get_n_items(); i++) {
			auto child = children->get_item(i);
			if (auto msg = child->saveDatabase()) return msg;
		}
		return nullptr;
	}

	sdk::utils::ReportMessagePtr saveFile() {
		if (isFileSaveRequired()) {
			if (auto msg = onSaveFile()) return msg;
			setFileSaveRequired(false);
		}
		if (!children) return nullptr;
		for (uint32_t i = 0; i < children->get_n_items(); i++) {
			auto child = children->get_item(i);
			if (auto msg = child->saveFile()) return msg;
		}
		return nullptr;
	}

	virtual std::filesystem::path getHeaderPath() const { return {}; }

	virtual std::filesystem::path getSourcePath() const { return {}; }

	virtual std::shared_ptr<mvp::IEditorPresenter> openEditor() { return nullptr; }

private:
	virtual sdk::utils::ReportMessagePtr onLoadDatabase() { return nullptr; }

	virtual sdk::utils::ReportMessagePtr onSaveDatabase() const { return nullptr; }

	virtual sdk::utils::ReportMessagePtr onSaveFile() const { return nullptr; }
};
} // namespace mer::editor::ui

#endif //FILESYSTEMENTRY_H
