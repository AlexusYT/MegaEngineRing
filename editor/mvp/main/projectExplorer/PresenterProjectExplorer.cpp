//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 12.08.24.
//

#include "PresenterProjectExplorer.h"

#include "IModelProjectExplorer.h"
#include "IViewProjectExplorer.h"
#include "entries/ProjectExplorerElement.h"
#include "project/Project.h"

namespace mer::editor::mvp {
PresenterProjectExplorer::PresenterProjectExplorer(const std::shared_ptr<IModelProjectExplorer> &pModel)
	: model(pModel) {
	for (auto view: views) {
		view->onPathChanged(model->getRootPath());
		view->setPresenter(this);
	}
}

void PresenterProjectExplorer::setEntrySelectionChanged(
	const sigc::slot<void(ui::ProjectExplorerEntry*)> & /*pEntrySelectionChanged*/) {
	//viewProjectExplorer->setEntrySelectionChanged(pEntrySelectionChanged);
}

void PresenterProjectExplorer::addView(const std::shared_ptr<IView> &pNewView) {
	const auto view = std::dynamic_pointer_cast<IViewProjectExplorer>(pNewView);
	if (!view) return;
	view->setSlotCreateModel([this](const Glib::RefPtr<Glib::ObjectBase> &pItem) -> Glib::RefPtr<Gio::ListModel> {
		if (const auto col = std::dynamic_pointer_cast<ProjectExplorerElement>(pItem)) { return col->getChildren(); }
		return model->getElements()->getChildren();
	});
	view->onPathChanged(model->getRootPath());
	view->setPresenter(this);
	views.push_back(view);
	view->openView();
}

void PresenterProjectExplorer::removeView(const std::shared_ptr<IView> &pOldView) { erase(views, pOldView); }

void PresenterProjectExplorer::run() {
	for (const auto &viewProjectExplorer: views) viewProjectExplorer->openView();
}

void PresenterProjectExplorer::stop() {
	for (const auto &viewProjectExplorer: views) viewProjectExplorer->closeView();
}
} // namespace mer::editor::mvp