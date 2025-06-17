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

#ifndef VIEWSETTINGSWINDOW_H
#define VIEWSETTINGSWINDOW_H
#include <typeindex>

#include "KwasarEngine/ui/UiPopup.h"
#include "KwasarEngine/ui/UiWindow.h"
#include "mvp/IView.h"

namespace ked {
class SettingsCategory;
class IPresenterSettingsWindow;

class IViewSettingsWindow : public IView {
public:
	~IViewSettingsWindow() override {}

	virtual void setPresenter(IPresenterSettingsWindow* pPresenter) = 0;

	virtual void openPopup() = 0;
};

class ViewSettingsWindow : public IViewSettingsWindow, public ke::UiPopup {
	IPresenterSettingsWindow* presenter{};
	std::shared_ptr<IWidgetContext> context{};
	std::shared_ptr<SettingsCategory> selectedCategory{};
	std::unordered_map<std::type_index, std::pair<std::shared_ptr<SettingsCategory> /*cloned*/,
												  std::shared_ptr<SettingsCategory> /*original*/>>
	clonedCategories{};
	bool settingsChanged{};

protected:
	explicit ViewSettingsWindow(const std::shared_ptr<IWidgetContext> &pContext)
		: UiPopup("ViewSettingsWindow"), context(pContext) {}

public:
	static std::shared_ptr<ViewSettingsWindow> create(const std::shared_ptr<IWidgetContext> &pContext) {
		return std::shared_ptr<ViewSettingsWindow>(new ViewSettingsWindow(pContext));
	}

	[[nodiscard]] std::string getTitle() const override { return trs("SettingsWindow"); }

	void openPopup() override;

	void onUpdate(bool pVisible) override;

	void setPresenter(IPresenterSettingsWindow* pPresenter) override { presenter = pPresenter; }

	void openView() override;

	void closeView() override;

private:
	void updateChangedState();

	void applyChanges();
};
} // namespace ked

#endif //VIEWSETTINGSWINDOW_H