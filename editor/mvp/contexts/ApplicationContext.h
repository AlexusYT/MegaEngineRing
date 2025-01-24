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
// Created by alexus on 10.08.24.
//

#ifndef GTKMMCONTEXT_H
#define GTKMMCONTEXT_H
#include "IWidgetContext.h"

namespace mer::editor::mvp {

class ApplicationContext : public IWidgetContext {
	std::shared_ptr<Gtk::Application> application;

	Gtk::Window* window;

	explicit ApplicationContext(const std::shared_ptr<Gtk::Application> &pApplication)
		: application(pApplication), window(nullptr) {}

public:
	static std::shared_ptr<IWidgetContext> create(const std::shared_ptr<Gtk::Application> &pApplication) {
		return std::shared_ptr<ApplicationContext>(new ApplicationContext(pApplication));
	}

	void addWidget(Gtk::Widget* pWidget) override;

	void removeWidget() override;

	void setTitle(const std::string &pTitle) override;
};


} // namespace mer::editor::mvp

#endif //GTKMMCONTEXT_H
