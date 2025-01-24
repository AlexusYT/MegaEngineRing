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

#ifndef IMODELCENTERWINDOW_H
#define IMODELCENTERWINDOW_H
#include <sigc++/functors/slot.h>

namespace mer::editor::mvp {
class ITabPlaceholder;
class ITab;
class IPresenter;

class IModelCenterWindow {
public:
	using TabInfo = std::pair<int, IPresenter*>;
	virtual ~IModelCenterWindow() = default;

	virtual void addPlaceholder(const std::shared_ptr<ITabPlaceholder> &pTab) = 0;

	virtual void addTab(ITabPlaceholder* pPlaceholder, ITab* pTab, IPresenter* pPresenter) = 0;

	virtual void removeOpenedTab(ITabPlaceholder* pTab) = 0;

	virtual bool isTabOpened(ITabPlaceholder* pTab) const = 0;

	virtual void setTabIndex(ITabPlaceholder* pPlaceholder, int pIndex) = 0;

	virtual int getTabIndex(ITabPlaceholder* pPlaceholder) = 0;

	virtual IPresenter* getPresenterByIndex(int pIndex) const = 0;

	[[nodiscard]] virtual const std::unordered_map<ITab*, TabInfo> &getOpenedTabs() const = 0;

	virtual void setOnSlotRemoving(const sigc::slot<void(int pIndex)> &pOnSlotRemoving) = 0;
};

} // namespace mer::editor::mvp

#endif //IMODELCENTERWINDOW_H
