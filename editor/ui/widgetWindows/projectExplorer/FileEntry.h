//
// Created by alexus on 27.01.24.
//

#ifndef FILEENTRY_H
#define FILEENTRY_H
#include "ProjectExplorerEntry.h"

namespace mer::editor::ui {
class FileEntry : public ProjectExplorerEntry {
	std::filesystem::path path;

protected:
	FileEntry(const std::filesystem::path &pPath) : ProjectExplorerEntry(pPath.filename()), path(pPath) {}

public:
	static std::shared_ptr<FileEntry> create(const std::string &pName) {
		return Glib::make_refptr_for_instance(new FileEntry(pName));
	}

	std::shared_ptr<mvp::IEditorPresenter> openEditor() override;
};

} // namespace mer::editor::ui

#endif //FILEENTRY_H
