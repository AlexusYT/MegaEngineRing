//
// Created by alexus on 29.01.24.
//

#ifndef APPINFOEDITOR_H
#define APPINFOEDITOR_H

#include "IViewAppInfo.h"

namespace mer::editor::project {
class ApplicationInfo;
class AppInfoProperty;
} // namespace mer::editor::project

namespace mer::editor::mvp {
class AppInfoEditor : public IViewAppInfo {
	ValueChangedSlot valueChangedSlot;
	std::unordered_map<Glib::RefPtr<Gtk::ListItem>, sigc::connection> connections;
	Gtk::Box main;
	ui::TreeWidget tree;


public:
	AppInfoEditor();

	void addWindow(const std::shared_ptr<Gtk::Window> &pWindow) override;

	void closeWindow() override {}

	void setSlotCreateModel(const ui::TreeWidget::SlotCreateModel &pSlotCreateModel) override;

	Gtk::Widget &getMainWidget() override { return main; }

	[[nodiscard]] const ValueChangedSlot &getValueChangedSlot() const override { return valueChangedSlot; }

	void setValueChangedSlot(const ValueChangedSlot &pValueChangedSlot) override {
		valueChangedSlot = pValueChangedSlot;
	}
};
} // namespace mer::editor::mvp


#endif //APPINFOEDITOR_H
