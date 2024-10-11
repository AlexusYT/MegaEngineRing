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
// Created by alexus on 10.10.24.
//

#include "PanedLayoutTab.h"

#include <nlohmann/json.hpp>

namespace mer::editor::mvp {
void PanedLayoutTab::deserialize(nlohmann::json &pJson) const {
	pJson["Name"] = name;
	pJson["LayoutFile"] = path;
	pJson["AssociatedWith"] = associatedWith;
}

void PanedLayoutTab::serialize(const nlohmann::json &pJson) {
	pJson.at("Name").get_to(name);
	pJson.at("LayoutFile").get_to(path);
	pJson.at("AssociatedWith").get_to(associatedWith);
}
} // namespace mer::editor::mvp