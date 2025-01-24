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
// Created by alexus on 10.01.24.
//

#ifndef TREEOBJECTWINDOW_H
#define TREEOBJECTWINDOW_H

#include "IViewObjectsTree.h"

namespace mer::editor::ui {
class CustomTreeView;
}
namespace mer::editor::mvp {
class IWidgetContext;
class ExplorerObject;

class ViewObjectsTree : public IViewObjectsTree {
public:
	using SlotEntrySelectionChanged = sigc::slot<void(mvp::ExplorerObject*)>;

private:
	Gtk::ScrolledWindow mainScrolledWindow;
	ui::CustomTreeView* tree;
	std::shared_ptr<IWidgetContext> context;

public:
	ViewObjectsTree(const std::shared_ptr<IWidgetContext> &pContext);

	void setTopLevelObjects(const std::shared_ptr<Gio::ListModel> &pTopLevelObjects) override;

	void openView() override;

	void closeView() override;

private:
	static std::shared_ptr<Gio::MenuItem> createItem(const std::string &pName, const std::string &pAction,
													 const Glib::VariantBase &pVariant);
};
} // namespace mer::editor::mvp

#endif //TREEOBJECTWINDOW_H
