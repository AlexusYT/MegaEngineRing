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

#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H

namespace mer::editor::mvp {
class IEditorPresenter;
} // namespace mer::editor::mvp

namespace mer::editor::ui {
class ProjectExplorerEntry;

class CenterWindow {
	Gtk::Notebook mainNotebook;

	struct Page {
		int pageNum;
		std::shared_ptr<mvp::IEditorPresenter> widget;
	};

	std::unordered_map<ProjectExplorerEntry* /*entry*/, Page /*pageInfo*/> openedPages;
	std::vector<Gtk::Window> windows;

public:
	CenterWindow();

	operator Gtk::Widget&() { return mainNotebook; }

	void openEntry(ProjectExplorerEntry* pEntry);
};
} // namespace mer::editor::ui


#endif //CENTERWINDOW_H
