//
// Created by alexus on 19.01.24.
//

#ifndef PROJECTEXPORERWINDOW_H
#define PROJECTEXPORERWINDOW_H
#include <ui/customWidgets/TreeWidget.h>

#include "ProjectExplorerEntry.h"

namespace n::core::project {
class Project;
}

namespace UI_CORE {
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
} // namespace UI_CORE

#endif //PROJECTEXPORERWINDOW_H
