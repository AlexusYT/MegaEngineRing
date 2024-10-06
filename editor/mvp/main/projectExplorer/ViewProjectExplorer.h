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

#ifndef PROJECTEXPORERWINDOW_H
#define PROJECTEXPORERWINDOW_H

#include "IViewProjectExplorer.h"

namespace mer::editor::ui {
class CustomTreeView;
}

namespace mer::editor::mvp {
class ProjectExplorerElement;

class ViewProjectExplorer : public IViewProjectExplorer {
	IPresenterProjectExplorer* presenter{};
	Gtk::ScrolledWindow mainScrolledWindow;
	ui::CustomTreeView* treeView;
	std::shared_ptr<IWidgetContext> context;

public:
	explicit ViewProjectExplorer(const std::shared_ptr<IWidgetContext> &pContext);

	void setSlotCreateModel(
		const sigc::slot<std::shared_ptr<Gio::ListModel>(const std::shared_ptr<Glib::ObjectBase> &)> &pSlot) override;

	void setSelectionChangedSlot(const sigc::slot<void(ProjectExplorerElement* pElement)> &pSlot) override;

	void setSelectOnDoubleClick(const bool pSelectOnDoubleClick) override;

	void openView() override;

	void closeView() override;

private:
	void addNameColumn() const;

	void addTypeColumn() const;

	void onPathChanged(const std::filesystem::path &pPath) const override;

	void selectByUri(const std::filesystem::path &pPath) override;

	static std::shared_ptr<Gio::MenuItem> createItem(const std::string &pName, const std::string &pAction,
													 const Glib::Variant<Glib::ustring> &pVariant);

	static std::optional<uint32_t> getPositionByUri(const std::filesystem::path &pPath,
													const std::shared_ptr<Gtk::TreeListRow> &pRow);

	[[nodiscard]] IPresenterProjectExplorer* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterProjectExplorer* pPresenter) override { presenter = pPresenter; }
};

} // namespace mer::editor::mvp

#endif //PROJECTEXPORERWINDOW_H
