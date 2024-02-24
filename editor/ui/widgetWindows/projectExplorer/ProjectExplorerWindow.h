//
// Created by alexus on 19.01.24.
//

#ifndef PROJECTEXPORERWINDOW_H
#define PROJECTEXPORERWINDOW_H
#include <ui/customWidgets/TreeWidget.h>

#include "ProjectExplorerEntry.h"

namespace mer::editor::project {
class Project;
}

namespace mer::editor::ui {
class ProjectExplorerWindow {
public:
	using SlotEntrySelectionChanged = sigc::slot<void(ProjectExplorerEntry*)>;

private:
	Gtk::ScrolledWindow mainScrolledWindow;
	TreeWidget tree;
	project::Project* project;
	SlotEntrySelectionChanged entrySelectionChanged;

public:
	ProjectExplorerWindow(project::Project* pProject);

	operator Gtk::Widget&() { return mainScrolledWindow; }

	[[nodiscard]] const SlotEntrySelectionChanged &getEntrySelectionChanged() const { return entrySelectionChanged; }

	void setEntrySelectionChanged(const SlotEntrySelectionChanged &pEntrySelectionChanged) {
		entrySelectionChanged = pEntrySelectionChanged;
	}
};
} // namespace mer::editor::ui

#endif //PROJECTEXPORERWINDOW_H
