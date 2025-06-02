//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 23.05.25.
//

#ifndef PRESENTERSETTINGSWINDOW_H
#define PRESENTERSETTINGSWINDOW_H
#include "mvp/IPresenter.h"

namespace mer::editor::mvp {

class IViewSettingsWindow;
class IModelSettingsWindow;

class IPresenterSettingsWindow : public IPresenter {
public:
	virtual void openPopup() = 0;
};

class PresenterSettingsWindow : public IPresenterSettingsWindow {
	std::shared_ptr<IViewSettingsWindow> view;
	std::shared_ptr<IModelSettingsWindow> model;

protected:
	PresenterSettingsWindow(const std::shared_ptr<IViewSettingsWindow> &pView,
							const std::shared_ptr<IModelSettingsWindow> &pModel)
		: view(pView), model(pModel) {}

public:
	static std::shared_ptr<IPresenterSettingsWindow> create(const std::shared_ptr<IViewSettingsWindow> &pView,
															const std::shared_ptr<IModelSettingsWindow> &pModel);

	void openPopup() override;

	void run() override;

	void stop() override;

	std::string getTypeName() override { return "PresenterSettingsWindow"; }
};

} // namespace mer::editor::mvp

#endif //PRESENTERSETTINGSWINDOW_H
