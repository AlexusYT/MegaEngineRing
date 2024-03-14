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
// Created by alexus on 06.02.24.
//

#ifndef VIEWSCENEEDITOR_H
#define VIEWSCENEEDITOR_H

#include "IViewSceneEditor.h"
#include "ui/widgetWindows/ObjectPropertiesWindow.h"
#include "ui/widgetWindows/TreeObjectWindow.h"

namespace mer::editor::mvp {
class IViewMain;

class ViewSceneEditor : public IViewSceneEditor {
	Gtk::Paned mainWidget;

	ui::TreeObjectWindow objectWindow;
	ui::ObjectPropertiesWindow propertiesWindow;

	Gtk::GLArea area;
	Gtk::Box loadingBox{Gtk::Orientation::VERTICAL};
	Gtk::Label loadingErrorLabel;
	Glib::RefPtr<Gtk::EventControllerMotion> motionController;
	IViewMain* viewMain{};
	Glib::RefPtr<Gdk::GLContext> sharedContext;

public:
	ViewSceneEditor();

	Gtk::Widget &getMainWidget() override { return mainWidget; }

	sigc::connection connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) override;

	sigc::connection connectRealize(const sigc::slot<void()> &pSlot) override;

	sigc::connection connectUnrealize(const sigc::slot<void()> &pSlot) override;

	sigc::connection connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) override;

	sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override;

	sigc::connection connectKeyReleasedSignal(
		const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const override;

	sigc::connection connectCursorMovedSignal(const sigc::slot<void(double pX, double pY)> &pSlot) const override {
		return motionController->signal_motion().connect(pSlot, false);
	}

	void makeCurrent() override;

	[[nodiscard]] const Glib::RefPtr<Gdk::GLContext> &getSharedContext() const override { return sharedContext; }

	void redraw() override;

	void throwIfError() override;

	void emitResize() override;

	void onLoadingStarted() override;

	void onLoadingStopped(const sdk::utils::ReportMessagePtr &pError) override;

	void onSceneReady(const std::shared_ptr<Gio::ListStore<ui::EditorSceneObject>> &pTopLevelObjects) override {
		objectWindow.setTopLevelObjects(pTopLevelObjects);
	}
};
} // namespace mer::editor::mvp

#endif //VIEWSCENEEDITOR_H
