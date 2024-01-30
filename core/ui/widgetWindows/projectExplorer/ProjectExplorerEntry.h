//
// Created by alexus on 19.01.24.
//

#ifndef FILESYSTEMENTRY_H
#define FILESYSTEMENTRY_H

namespace MVP_CORE {
class IEditorPresenter;
class IEditor;
} // namespace MVP_CORE

namespace UI_CORE {

enum class EntryKind { FILE, DIRECTORY, SCENE };

class ProjectExplorerEntry : public Glib::Object {
	std::string name;
	EntryKind kind{EntryKind::DIRECTORY};
	std::string image;
	Glib::RefPtr<Gio::ListStore<ProjectExplorerEntry>> children;
	Glib::RefPtr<Gio::Menu> contextMenu;

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

	engine::utils::ReportMessageUPtr load() {
		if (auto msg = onLoad()) return msg;
		if (!children) return nullptr;
		for (uint32_t i = 0; i < children->get_n_items(); i++) {
			auto child = children->get_item(i);
			if (auto msg = child->load()) return msg;
		}
		return nullptr;
	}

	engine::utils::ReportMessageUPtr save() const {
		if (auto msg = onSave()) return msg;
		if (!children) return nullptr;
		for (uint32_t i = 0; i < children->get_n_items(); i++) {
			auto child = children->get_item(i);
			if (auto msg = child->save()) return msg;
		}
		return nullptr;
	}

	virtual std::filesystem::path getPath() const { return {}; }

	virtual std::shared_ptr<mvp::IEditorPresenter> openEditor() { return nullptr; }

private:
	virtual engine::utils::ReportMessageUPtr onLoad() { return nullptr; }

	virtual engine::utils::ReportMessageUPtr onSave() const { return nullptr; }
};
} // namespace UI_CORE

#endif //FILESYSTEMENTRY_H
