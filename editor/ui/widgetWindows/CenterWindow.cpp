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

#include "CenterWindow.h"

#include <mvp/main/editors/IEditorPresenter.h>

#include "projectExplorer/ProjectExplorerEntry.h"

static GtkNotebook* Notebook_signal_create_window_callback(GtkNotebook* self, GtkWidget* p0, void* data) {
	using namespace Gtk;
	using SlotType = sigc::slot<Glib::RefPtr<Notebook>(Widget*)>;

	auto obj = dynamic_cast<Notebook*>(Glib::ObjectBase::_get_current_wrapper((GObject*) self));
	// Do not try to call a signal on a disassociated wrapper.
	if (obj) {
		try {

			if (const auto slot = Glib::SignalProxyNormal::data_to_slot(data))
				return Glib::unwrap((*static_cast<SlotType*>(slot))(Glib::wrap(p0)));
		} catch (...) { Glib::exception_handlers_invoke(); }
	}
	using RType = GtkNotebook*;
	return RType();
}

static const Glib::SignalProxyInfo Notebook_signal_create_window = {
	"create_window", (GCallback) &Notebook_signal_create_window_callback,
	(GCallback) &Notebook_signal_create_window_callback};

namespace mer::editor::ui {
CenterWindow::CenterWindow() {
	//g_signal_connect(mainNotebook.gobj(), "create_window", )
	Glib::SignalProxy<Glib::RefPtr<Gtk::Notebook>(Gtk::Widget*)>(&mainNotebook, &Notebook_signal_create_window)
		.connect(
			[this](Gtk::Widget* pWidget) -> Glib::RefPtr<Gtk::Notebook> {
				Gtk::Window &window = windows.emplace_back();
				;
				window.set_transient_for(*dynamic_cast<Gtk::Window*>(pWidget->get_root()));
				auto notebook = Glib::make_refptr_for_instance(new Gtk::Notebook);
				window.set_child(*notebook);
				window.set_size_request(400, 400);
				window.set_visible();
				notebook->set_group_name("center-window-notebook");
				notebook->signal_page_removed().connect(sigc::bind(
					[](Gtk::Widget* /*pPage*/, guint /*pPageNum*/, Gtk::Notebook* pNotebook, Gtk::Window* pWindow) {
						if (pNotebook->get_n_pages() == 0) pWindow->destroy();
					},
					notebook.get(), &window));
				sdk::utils::Logger::out("Create window");

				return notebook;
			},
			true);
	mainNotebook.set_group_name("center-window-notebook");
	mainNotebook.set_show_border(false);
}

void CenterWindow::openEntry(ProjectExplorerEntry* pEntry) {
	int pageNum;
	const auto iter = openedPages.find(pEntry);
	if (iter == openedPages.end()) {

		auto editor = pEntry->openEditor();
		if (!editor) return;
		pEntry->setCenterWindow(this);
		Gtk::Box box;
		auto fileNameLabel = Gtk::make_managed<Gtk::Label>(pEntry->getName());
		box.append(*fileNameLabel);
		Gtk::Button closeBtn;
		closeBtn.set_has_frame(false);
		closeBtn.set_icon_name("window-close");
		box.append(closeBtn);
		Gtk::Widget &editorWidget = *editor;
		pageNum = mainNotebook.append_page(editorWidget, box);
		editor->connectTabHeaderChanged([fileNameLabel](const std::string &pName) { fileNameLabel->set_label(pName); });

		openedPages.emplace_hint(iter, pEntry, Page{pageNum, editor});

		auto page = mainNotebook.get_page(*editor);
		page->property_reorderable().set_value(true);
		executeInMainThread([&editorWidget](const std::promise<void> &) { editorWidget.grab_focus(); });
		//page->property_detachable().set_value(true);
	} else {
		pageNum = iter->second.pageNum;

		Gtk::Widget &editorWidget = *iter->second.widget;
		executeInMainThread([&editorWidget](const std::promise<void> &) { editorWidget.grab_focus(); });
	}
	mainNotebook.set_current_page(pageNum);
}

} // namespace mer::editor::ui