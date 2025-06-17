//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 19.01.24.
//

#ifndef PROJECTEXPORERWINDOW_H
#define PROJECTEXPORERWINDOW_H

#include "KwasarEngine/ui/UiWindow.h"
#include "IViewProjectExplorer.h"

namespace ked {}

namespace ked {
class ConfirmationDialog;
class ProjectExplorerElement;

class ViewProjectExplorer : public IViewProjectExplorer, public ke::UiWindow {
	IPresenterProjectExplorer* presenter{};
	std::shared_ptr<IWidgetContext> context;
	std::vector<std::shared_ptr<ProjectExplorerElement>> array;

	std::shared_ptr<ProjectExplorerElement> selectedElement{};
	std::shared_ptr<ConfirmationDialog> dialog;

public:
	explicit ViewProjectExplorer(const std::shared_ptr<IWidgetContext> &pContext);

	void setElements(const std::vector<std::shared_ptr<ProjectExplorerElement>> &pArray) override;

	[[nodiscard]] std::string getTitle() const override { return trs("ProjectExplorerTool"); }

	void openView() override;

	void closeView() override;

	void onUpdate(bool pVisible) override;

	void startConfirmation(const std::string &pTitle, const std::string &pMessage,
						   const std::function<void(int pId)> &pResult,
						   const std::vector<std::string> &pButtons) override;

private:
	void updateTreeLevel(const std::vector<std::shared_ptr<ProjectExplorerElement>> &pElements);

	[[nodiscard]] IPresenterProjectExplorer* getPresenter() const override { return presenter; }

	void setPresenter(IPresenterProjectExplorer* pPresenter) override { presenter = pPresenter; }
};
} // namespace ked

#endif //PROJECTEXPORERWINDOW_H