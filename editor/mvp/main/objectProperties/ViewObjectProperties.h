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
// Created by alexus on 17.01.24.
//

#ifndef OBJECTPROPERTIESWINDOW_H
#define OBJECTPROPERTIESWINDOW_H
#include "IViewObjectProperties.h"

namespace mer::editor::ui {
class CustomTreeView;
}

namespace mer::editor::mvp {
class ExplorerObject;

class ViewObjectProperties : public IViewObjectProperties {
	Gtk::Box mainWidget;
	Gtk::MenuButton addBtn;
	Gtk::Button removeBtn;
	ui::CustomTreeView* propertiesTree;
	std::shared_ptr<IWidgetContext> context;

public:
	ViewObjectProperties(const std::shared_ptr<IWidgetContext> &pContext);

	void setObject(ExplorerObject* pObject) override;

	static std::shared_ptr<Gtk::ColumnViewColumn> createNameColumn();

	static std::shared_ptr<Gtk::ColumnViewColumn> createValueColumn();

private:
	void openView() override;

	void closeView() override;
};

} // namespace mer::editor::mvp


#endif //OBJECTPROPERTIESWINDOW_H
