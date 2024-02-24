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
