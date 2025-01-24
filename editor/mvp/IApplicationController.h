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
// Created by alexus on 09.08.24.
//

#ifndef IAPPLICATIONCONTROLLER_H
#define IAPPLICATIONCONTROLLER_H

#include <gtkmm/application.h>

namespace mer::editor::mvp {
class IWidgetContext;
class IPresenter;

class IApplicationController {
public:
	virtual ~IApplicationController() = default;

	virtual void run(const std::shared_ptr<IPresenter> &pPresenter) = 0;

	virtual void stop(IPresenter* pPresenter) = 0;

	virtual std::shared_ptr<IPresenter> getPresenterByName(const std::string &pName) = 0;

	[[nodiscard]] virtual const std::shared_ptr<Gtk::Application> &getApp() const = 0;

	virtual void setApp(const std::shared_ptr<Gtk::Application> &pApp) = 0;
};
} // namespace mer::editor::mvp


#endif //IAPPLICATIONCONTROLLER_H
