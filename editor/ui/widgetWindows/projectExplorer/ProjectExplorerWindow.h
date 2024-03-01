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
