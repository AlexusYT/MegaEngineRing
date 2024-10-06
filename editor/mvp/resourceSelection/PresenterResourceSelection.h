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
// Created by alexus on 30.09.24.
//

#ifndef PRESENTERRESOURCESELECTION_H
#define PRESENTERRESOURCESELECTION_H
#include "IPresenterResourceSelection.h"

namespace mer::editor::mvp {
class FileResourceReader;
class IViewResourceSelection;
} // namespace mer::editor::mvp

namespace mer::editor::mvp {
class IModelResourceSelection;
}

namespace mer::editor::mvp {

class PresenterResourceSelection : public IPresenterResourceSelection {
	std::shared_ptr<IModelResourceSelection> model;
	std::shared_ptr<IViewResourceSelection> view;
	std::shared_ptr<FileResourceReader> resourceReader;

public:
	PresenterResourceSelection(const std::shared_ptr<IModelResourceSelection> &pModel,
							   const std::shared_ptr<IViewResourceSelection> &pView);

	void selectClicked() override;

	void onCancelClicked() override;

	void unsetClicked() override;

	void onElementSelectionChanged(ProjectExplorerElement* pElement) override;

	void onObjectSelectionChanged(const std::string &pObjectName) override;

	void run() override;

	void stop() override;
};

} // namespace mer::editor::mvp

#endif //PRESENTERRESOURCESELECTION_H
