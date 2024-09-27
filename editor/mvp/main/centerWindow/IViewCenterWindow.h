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
// Created by alexus on 15.08.24.
//

#ifndef IVIEWCENTERWINDOW_H
#define IVIEWCENTERWINDOW_H

#include "mvp/IView.h"

namespace mer::editor::mvp {
class IPresenterCenterWindow;

class IViewCenterWindow : public IView {
public:
	~IViewCenterWindow() override = default;

	virtual int addTab(Gtk::Widget &pContent) = 0;

	virtual void setTabTitle(int pIndex, const std::string &pTitle) = 0;

	virtual sigc::connection connectPageIndexChanged(Gtk::Widget &pContent,
													 const sigc::slot<void(int pNewIndex)> &pSlot) = 0;

	virtual void removeTab(int pIndex) = 0;

	virtual void switchToTab(int pIndex) = 0;

	virtual void setPresenter(IPresenterCenterWindow* pPresenter) = 0;
};

} // namespace mer::editor::mvp

#endif //IVIEWCENTERWINDOW_H
