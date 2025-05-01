//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024-2025. Timofeev (Alexus_XX) Alexander
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 19.01.24.
//

#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H
#include "IViewCenterWindow.h"
#include "mvp/ThreadDispatcher.h"

namespace mer::editor::mvp {
class IPresenterSceneEditor;
} // namespace mer::editor::mvp

namespace mer::editor::ui {

class ProjectExplorerEntry;
}

namespace mer::editor::mvp {

class ViewCenterWindow : public IViewCenterWindow, public ThreadDispatcher {
	IPresenterCenterWindow* presenter{};
	Gtk::Notebook mainNotebook;

	struct Page {
		int pageNum;
	};

	std::unordered_map<ui::ProjectExplorerEntry* /*entry*/, Page /*pageInfo*/> openedPages;
	std::vector<Gtk::Window> windows;
	std::shared_ptr<IWidgetContext> context;

public:
	ViewCenterWindow(const std::shared_ptr<IWidgetContext> &pContext);

	void openEntry(ui::ProjectExplorerEntry* pEntry);

	int addTab(Gtk::Widget &pContent) override;

	void setTabTitle(int pIndex, const std::string &pTitle) override;

	sigc::connection connectPageIndexChanged(Gtk::Widget &pContent,
											 const sigc::slot<void(int pNewIndex)> &pSlot) override;

	void removeTab(int pIndex) override;

	void switchToTab(int pIndex) override;

	void openView() override;

	void closeView() override;

	void setPresenter(IPresenterCenterWindow* pPresenter) override { presenter = pPresenter; }
};
} // namespace mer::editor::mvp


#endif //CENTERWINDOW_H
