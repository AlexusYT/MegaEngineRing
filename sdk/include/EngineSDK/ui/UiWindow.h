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
// Created by alexus on 10.02.25.
//

#ifndef UIWINDOW_H
#define UIWINDOW_H
#include "EngineSDK/render/IRenderable.h"
#include "UiBase.h"
#include "imgui.h"

namespace mer::sdk {
class UiPopup;
}

namespace mer::sdk {

class UiWindow : public UiBase {

	std::vector<std::shared_ptr<UiPopup>> popups;

public:
	UiWindow(const std::string &pName, const std::string &pTitle) : UiBase(pName, pTitle) {}

	static constexpr float POSITION_CENTERED = -100000.0f;
	static constexpr float SIZE_MATH_PARENT = -1.0f;

	void render() override;
	void addPopup(const std::shared_ptr<UiPopup> &pPopup);
};

} // namespace mer::sdk

#endif //UIWINDOW_H
