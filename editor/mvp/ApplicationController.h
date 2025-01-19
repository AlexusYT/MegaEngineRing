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

#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include "IApplicationController.h"
#include "IPresenter.h"

namespace mer::editor::mvp {
class ApplicationController : public IApplicationController {
	std::unordered_map<std::string /*typeName*/, std::shared_ptr<IPresenter>> presenters;
	std::shared_ptr<Gtk::Application> app;

public:
	explicit ApplicationController();

	~ApplicationController() override;

	void run(const std::shared_ptr<IPresenter> &pPresenter) override;

	void stop(IPresenter* pPresenter) override;

	std::shared_ptr<IPresenter> getPresenterByName(const std::string &pName) override {
		const auto iter = presenters.find(pName);
		if (iter == presenters.end()) { return nullptr; }
		return iter->second;
	}

	[[nodiscard]] const std::shared_ptr<Gtk::Application> &getApp() const final { return app; }


private:
	void setApp(const std::shared_ptr<Gtk::Application> &pApp) final { app = pApp; }
};
} // namespace mer::editor::mvp


#endif //APPLICATIONCONTROLLER_H
