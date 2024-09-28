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
// Created by alexus on 10.08.24.
//

#include "ApplicationContext.h"

namespace mer::editor::mvp {

void ApplicationContext::addWidget(Gtk::Widget* pWidget) {

	if (auto windowWidget = dynamic_cast<Gtk::Window*>(pWidget)) {
		window = windowWidget;
	} else {
		window = Gtk::make_managed<Gtk::ApplicationWindow>();
		window->set_child(*pWidget);
	}
	window->set_application(application);
	window->set_visible(true);
}

void ApplicationContext::removeWidget() {
	if (!window || !window->gobj()) return;
	window->set_visible(false);
}

void ApplicationContext::setTitle(const std::string &pTitle) {
	if (!window) return;
	window->set_title(pTitle);
}
} // namespace mer::editor::mvp