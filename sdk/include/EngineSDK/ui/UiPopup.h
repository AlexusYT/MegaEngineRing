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
// Created by alexus on 14.02.25.
//

#ifndef UIPOPUP_H
#define UIPOPUP_H
#include "UiBase.h"

namespace mer::sdk {

class UiPopup : public UiBase {

public:
	UiPopup(const std::string &pName, const std::string &pTitle) : UiBase(pName, pTitle) {}

	void render() override;
};

} // namespace mer::sdk

#endif //UIPOPUP_H
