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
// Created by alexus on 30.09.24.
//

#ifndef IPRESENTERRESOURCESELECTION_H
#define IPRESENTERRESOURCESELECTION_H
#include "mvp/PresenterBase.h"
#include "mvp/main/projectExplorer/entries/ProjectExplorerElement.h"

namespace mer::editor::mvp {

class IPresenterResourceSelection : public PresenterBase {
public:
	virtual void selectClicked() = 0;

	virtual void onCancelClicked() = 0;

	virtual void unsetClicked() = 0;

	virtual void onElementSelectionChanged(ProjectExplorerElement* pElement) = 0;

	virtual void onObjectSelectionChanged(const std::string &pObjectName) = 0;
};

} // namespace mer::editor::mvp

#endif //IPRESENTERRESOURCESELECTION_H
