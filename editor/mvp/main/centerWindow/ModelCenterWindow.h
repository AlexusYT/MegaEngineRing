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

#ifndef MODELCENTERWINDOW_H
#define MODELCENTERWINDOW_H
#include "IModelCenterWindow.h"

namespace mer::editor::mvp {

class ModelCenterWindow : public IModelCenterWindow {
	std::unordered_map<ITabPlaceholder*, ITab*> tabByPlaceholder;
	std::unordered_map<ITab*, TabInfo> openedTabs;
	std::unordered_map<int /*index*/, IPresenter*> presenterByIndex;
	std::set<std::shared_ptr<ITabPlaceholder>> placeholders;
	sigc::slot<void(int pIndex)> onSlotRemoving;

public:
	void addPlaceholder(const std::shared_ptr<ITabPlaceholder> &pTab) override;

	void addTab(ITabPlaceholder* pPlaceholder, ITab* pTab, IPresenter* pPresenter) override;

	void removeOpenedTab(ITabPlaceholder* pTabToRemove) override;

	bool isTabOpened(ITabPlaceholder* pTab) const override;

	void setTabIndex(ITabPlaceholder* pPlaceholder, int pIndex) override;

	int getTabIndex(ITabPlaceholder* pPlaceholder) override;

	IPresenter* getPresenterByIndex(int pIndex) const override;

	[[nodiscard]] const std::unordered_map<ITab*, TabInfo> &getOpenedTabs() const override { return openedTabs; }

	void setOnSlotRemoving(const sigc::slot<void(int pIndex)> &pOnSlotRemoving) override {
		onSlotRemoving = pOnSlotRemoving;
	}
};

} // namespace mer::editor::mvp

#endif //MODELCENTERWINDOW_H
