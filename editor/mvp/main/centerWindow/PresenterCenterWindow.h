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

#ifndef PRESENTERCENTERRWINDOW_H
#define PRESENTERCENTERRWINDOW_H
#include "IPresenterCenterWindow.h"

namespace mer::editor::ui {
class ProjectExplorerEntry;
}

namespace mer::editor::mvp {
class IViewCenterWindow;
class IModelCenterWindow;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {

class PresenterCenterWindow : public IPresenterCenterWindow {

	std::shared_ptr<IViewCenterWindow> viewCenterWindow;
	std::shared_ptr<IModelCenterWindow> modelCenterWindow;

public:
	PresenterCenterWindow(const std::shared_ptr<IViewCenterWindow> &pViewCenterWindow,
						  const std::shared_ptr<IModelCenterWindow> &pModelCenterWindow);

	void openTab(ITab* pTab) override;

	void closeTab(int pIndex) override;

	std::shared_ptr<TabPlaceholder> newPlaceholder();

private:
	void run() override;

	void stop() override;

	void onTabPopulating(TabPlaceholder* pPlaceholder, Gtk::Widget &pWidget) override;

	void onTabTitleChanging(TabPlaceholder* pPlaceholder, const std::string &pTitle) override;

	void onTabRemoving(TabPlaceholder* pPlaceholder) override;

	std::string getTypeName() override {
		return "PresenterCenterWindow";
	}
};

} // namespace mer::editor::mvp

#endif //PRESENTERCENTERRWINDOW_H
