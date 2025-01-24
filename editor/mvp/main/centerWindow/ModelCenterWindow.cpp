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

#include "ModelCenterWindow.h"

namespace mer::editor::mvp {

void ModelCenterWindow::addPlaceholder(const std::shared_ptr<ITabPlaceholder> &pTab) { placeholders.emplace(pTab); }

void ModelCenterWindow::addTab(ITabPlaceholder* pPlaceholder, ITab* pTab, IPresenter* pPresenter) {
	tabByPlaceholder.emplace(pPlaceholder, pTab);
	openedTabs.emplace(pTab, std::make_pair(-1, pPresenter));
}

void ModelCenterWindow::removeOpenedTab(ITabPlaceholder* pTabToRemove) {

	const auto tab = tabByPlaceholder.at(pTabToRemove);
	auto tabInfo = openedTabs.at(tab);
	int index = tabInfo.first;
	presenterByIndex.erase(index);
	openedTabs.erase(tab);
	tabByPlaceholder.erase(pTabToRemove);
	erase_if(placeholders,
			 [pTabToRemove](const std::shared_ptr<ITabPlaceholder> &pTab) { return pTab.get() == pTabToRemove; });
	if (onSlotRemoving) onSlotRemoving(index);
}

bool ModelCenterWindow::isTabOpened(ITabPlaceholder* /*pTab*/) const { return /*openedTabs.contains(pTab)*/ false; }

void ModelCenterWindow::setTabIndex(ITabPlaceholder* pPlaceholder, const int pIndex) {

	const auto tab = tabByPlaceholder.at(pPlaceholder);
	auto tabInfo = openedTabs.at(tab);
	auto prevIndex = tabInfo.first;
	if (prevIndex == pIndex) return;
	if (prevIndex >= 0) {
		if (auto iter = presenterByIndex.find(prevIndex); iter != presenterByIndex.end()) presenterByIndex.erase(iter);
	}

	presenterByIndex.emplace(pIndex, tabInfo.second);
	openedTabs.at(tab).first = pIndex;
}

int ModelCenterWindow::getTabIndex(ITabPlaceholder* pPlaceholder) {
	const auto tab = tabByPlaceholder.at(pPlaceholder);
	auto tabInfo = openedTabs.at(tab);
	return tabInfo.first;
}

IPresenter* ModelCenterWindow::getPresenterByIndex(const int pIndex) const { return presenterByIndex.at(pIndex); }
} // namespace mer::editor::mvp