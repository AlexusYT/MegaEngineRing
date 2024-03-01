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
#include <project/sceneObjects/SceneObject.h>
#include <ui/customWidgets/TreeWidget.h>

namespace mer::editor::ui {

class TreeObjectWindow {
	Gtk::Box mainBox;
	Gtk::Box buttonBox;
	Gtk::Button addBtn;
	Gtk::Button removeBtn;
	TreeWidget tree;

	Glib::RefPtr<Gtk::GestureClick> rightClickGesture;

	using SlotGetObjectChildren = sigc::slot<std::vector<std::shared_ptr<SceneObject>>(const SceneObject* pObject)>;
	SlotGetObjectChildren slotGetObjectChildren;


public:
	TreeObjectWindow();

	operator Gtk::Widget&() { return mainBox; }

	[[nodiscard]] const SlotGetObjectChildren &getSlotGetObjectChildren() const { return slotGetObjectChildren; }

	void setSlotGetObjectChildren(const SlotGetObjectChildren &pSlotGetObjectChildren);

	void removeObject(SceneObject* pObject);

	SceneObject* getSelectedObject();

private:
	SceneObject* getPopoverObject();

	static Glib::RefPtr<Gio::Menu> createContextMenu();
	static Glib::RefPtr<Gio::Menu> createAddContextMenu();
};
} // namespace mer::editor::ui


#endif //TREEOBJECTWINDOW_H
