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
// Created by alexus on 14.10.24.
//

#include "UiUtils.h"

namespace mer::editor::ui {
std::shared_ptr<Gio::MenuItem> UiUtils::createMenuItem(const std::string &pName, const std::string &pAction,
													   const Glib::VariantBase &pVariant) {
	std::shared_ptr<Gio::MenuItem> item = Gio::MenuItem::create(pName, "");
	item->set_action_and_target(pAction, pVariant);
	return item;
}
} // namespace mer::editor::ui