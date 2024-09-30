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

#ifndef VIEWRESOURCECREATION_H
#define VIEWRESOURCECREATION_H
#include "IViewResourceCreation.h"

namespace mer::editor::mvp {

class ViewResourceCreation : public IViewResourceCreation {
	std::shared_ptr<IWidgetContext> context;

public:
	explicit ViewResourceCreation(const std::shared_ptr<IWidgetContext> &pContext) : context(pContext) {}

	void openView() override;

	void closeView() override;
};

} // namespace mer::editor::mvp

#endif //VIEWRESOURCECREATION_H
