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
// Created by alexus on 15.08.24.
//

#include "PresenterCenterWindow.h"

#include "IModelCenterWindow.h"
#include "ITab.h"
#include "IViewCenterWindow.h"
#include "TabPlaceholder.h"
#include "mvp/ApplicationController.h"

namespace mer::editor::mvp {

PresenterCenterWindow::PresenterCenterWindow(const std::shared_ptr<IViewCenterWindow> &pViewCenterWindow,
											 const std::shared_ptr<IModelCenterWindow> &pModelCenterWindow)
	: viewCenterWindow(pViewCenterWindow), modelCenterWindow(pModelCenterWindow) {
	viewCenterWindow->setPresenter(this);
	modelCenterWindow->setOnSlotRemoving([this](const int pIndex) { viewCenterWindow->removeTab(pIndex); });
}

void PresenterCenterWindow::openTab(ITab* pTab) {
	auto openedTabs = modelCenterWindow->getOpenedTabs();
	if (const auto iter = openedTabs.find(pTab); iter != openedTabs.end()) {
		if (iter->second.first >= 0) viewCenterWindow->switchToTab(iter->second.first);
	} else {
		auto placeholder = newPlaceholder();
		modelCenterWindow->addPlaceholder(placeholder);
		auto presenter = pTab->createEditorPresenter(placeholder);
		modelCenterWindow->addTab(placeholder.get(), pTab, presenter.get());
		getAppController()->run(presenter);
	}
}

void PresenterCenterWindow::closeTab(const int pIndex) {
	auto presenter = modelCenterWindow->getPresenterByIndex(pIndex);
	getAppController()->stop(presenter);
}

std::shared_ptr<TabPlaceholder> PresenterCenterWindow::newPlaceholder() {
	auto placeholder = std::make_shared<TabPlaceholder>();
	auto interface = std::dynamic_pointer_cast<ITabPlaceholder>(placeholder);
	interface->setPresenter(this);
	return placeholder;
}

void PresenterCenterWindow::run() { viewCenterWindow->openView(); }

void PresenterCenterWindow::stop() { viewCenterWindow->closeView(); }

void PresenterCenterWindow::onTabPopulating(TabPlaceholder* pPlaceholder, Gtk::Widget &pWidget) {
	const int index = viewCenterWindow->addTab(pWidget);
	modelCenterWindow->setTabIndex(pPlaceholder, index);

	viewCenterWindow->connectPageIndexChanged(pWidget, [this, pPlaceholder](const int pNewIndex) {
		modelCenterWindow->setTabIndex(pPlaceholder, pNewIndex);
	});
}

void PresenterCenterWindow::onTabTitleChanging(TabPlaceholder* pPlaceholder, const std::string &pTitle) {
	const auto index = modelCenterWindow->getTabIndex(pPlaceholder);
	viewCenterWindow->setTabTitle(index, pTitle);
}

void PresenterCenterWindow::onTabRemoving(TabPlaceholder* pPlaceholder) {
	modelCenterWindow->removeOpenedTab(pPlaceholder);
}

} // namespace mer::editor::mvp