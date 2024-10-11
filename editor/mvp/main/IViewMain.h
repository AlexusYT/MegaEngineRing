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
// Created by alexus on 26.01.24.
//

#ifndef IVIEWMAIN_H
#define IVIEWMAIN_H
#include <mvp/IView.h>

namespace mer::editor::ui {
class ProjectExplorerEntry;
class MultiPaned;
} // namespace mer::editor::ui

namespace mer::editor::mvp {
class PanedLayoutTab;
class IPresenterMain;

class IViewMain : public IView {
public:
	virtual sigc::connection connectKeyPressedSignal(
		const sigc::slot<bool(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const = 0;

	virtual sigc::connection connectKeyReleasedSignal(
		const sigc::slot<void(guint pKeyVal, guint pKeyCode, Gdk::ModifierType pState)> &pSlot) const = 0;

	virtual void setWindowTitle(const std::string &pTitle) = 0;

	virtual void addActionGroup(const std::string &pName, const Glib::RefPtr<Gio::SimpleActionGroup> &pActionGroup) = 0;

	virtual void reportError(const sdk::utils::ReportMessagePtr &pError) = 0;

	virtual void setPresenter(IPresenterMain* pPresenter) = 0;

	virtual Gtk::Window* getWindow() = 0;

	virtual void setTabs(const std::vector<PanedLayoutTab> &pPanedLayoutsTabs) = 0;

	virtual void openTab(int32_t pTabIndex) = 0;

	virtual ui::MultiPaned* getMultiPanedByIndex(int32_t pIndex) = 0;
};
} // namespace mer::editor::mvp


#endif //IVIEWMAIN_H
