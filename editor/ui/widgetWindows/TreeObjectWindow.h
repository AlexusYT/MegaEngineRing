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
// Created by alexus on 10.01.24.
//

#ifndef TREEOBJECTWINDOW_H
#define TREEOBJECTWINDOW_H

#include <ui/customWidgets/TreeWidget.h>

namespace mer::editor::project {
class Project;
}

namespace mer::editor::ui {
class EditorSceneObject;

class TreeObjectWindow {
public:
	using SlotEntrySelectionChanged = sigc::slot<void(EditorSceneObject*)>;

private:
	Gtk::ScrolledWindow mainScrolledWindow;
	TreeWidget tree;
	SlotEntrySelectionChanged entrySelectionChanged;

public:
	TreeObjectWindow();

	operator Gtk::Widget&() { return mainScrolledWindow; }

	[[nodiscard]] const SlotEntrySelectionChanged &getEntrySelectionChanged() const { return entrySelectionChanged; }

	void setEntrySelectionChanged(const SlotEntrySelectionChanged &pEntrySelectionChanged) {
		entrySelectionChanged = pEntrySelectionChanged;
	}

	void setTopLevelObjects(const std::shared_ptr<Gio::ListStore<EditorSceneObject>> &pTopLevelObjects);
};
} // namespace mer::editor::ui

#endif //TREEOBJECTWINDOW_H
