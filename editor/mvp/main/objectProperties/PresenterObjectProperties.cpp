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
// Created by alexus on 26.09.24.
//

#include "PresenterObjectProperties.h"

#include "IModelObjectProperties.h"
#include "IViewObjectProperties.h"
#include "project/LoadedScene.h"

namespace mer::editor::mvp {
void PresenterObjectProperties::run() {
	for (const auto &view: views) { view->openView(); };
}

void PresenterObjectProperties::stop() {
	for (const auto &view: views) { view->closeView(); }
	views.clear();
	model.reset();
}

void PresenterObjectProperties::addView(const std::shared_ptr<IView> &pElement) {

	const auto view = std::dynamic_pointer_cast<IViewObjectProperties>(pElement);
	if (!view) return;
	views.push_back(view);
	auto loadedScene = model->getLoadedScene();
	loadedScene->connectSelectionChanged([view](ExplorerObject* pSelectedObject) { view->setObject(pSelectedObject); });
	view->openView();
}
} // namespace mer::editor::mvp