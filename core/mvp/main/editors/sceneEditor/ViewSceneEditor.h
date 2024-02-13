//
// Created by alexus on 06.02.24.
//

#ifndef VIEWSCENEEDITOR_H
#define VIEWSCENEEDITOR_H

#include "IViewSceneEditor.h"
#include "ui/widgetWindows/ObjectPropertiesWindow.h"
#include "ui/widgetWindows/TreeObjectWindow.h"

namespace MVP_CORE {
class ViewSceneEditor : public IViewSceneEditor {
	Gtk::Paned mainWidget;

	ui::TreeObjectWindow objectWindow;
	ui::ObjectPropertiesWindow propertiesWindow;

	Gtk::GLArea area;
	Gtk::Box loadingBox{Gtk::Orientation::VERTICAL};
	Gtk::Label loadingErrorLabel;


public:
	ViewSceneEditor();

	Gtk::Widget &getMainWidget() override { return mainWidget; }

	sigc::connection connectRender(const sigc::slot<bool(const Glib::RefPtr<Gdk::GLContext> &)> &pSlot) override;

	sigc::connection connectRealize(const sigc::slot<void()> &pSlot) override;

	sigc::connection connectUnrealize(const sigc::slot<void()> &pSlot) override;

	sigc::connection connectResize(const sigc::slot<void(int pWidth, int pHeight)> &pSlot) override;

	void makeCurrent() override;

	void redraw() override;

	void throwIfError() override;

	void onLoadingStarted() override;

	void onLoadingStopped(const engine::utils::ReportMessagePtr &pError) override;
};
} // namespace MVP_CORE

#endif //VIEWSCENEEDITOR_H
