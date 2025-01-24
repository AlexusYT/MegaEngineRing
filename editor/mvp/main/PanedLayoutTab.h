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
// Created by alexus on 10.10.24.
//

#ifndef PANEDLAYOUTTABS_H
#define PANEDLAYOUTTABS_H
#include <nlohmann/json_fwd.hpp>

namespace mer::editor::mvp {
class PanedLayoutTab {
	std::string name;
	std::string path;
	std::vector<std::string> associatedWith;

public:
	[[nodiscard]] const std::string &getName() const { return name; }

	void setName(const std::string &pName) { name = pName; }

	[[nodiscard]] const std::string &getPath() const { return path; }

	void setPath(const std::string &pPath) { path = pPath; }

	[[nodiscard]] const std::vector<std::string> &getAssociatedWith() const { return associatedWith; }

	void setAssociatedWith(const std::vector<std::string> &pAssociatedWith) { associatedWith = pAssociatedWith; }

	friend void to_json(nlohmann::json &pJson, const PanedLayoutTab &pType) { pType.deserialize(pJson); }

	friend void from_json(const nlohmann::json &pJson, PanedLayoutTab &pType) { pType.serialize(pJson); }

private:
	void deserialize(nlohmann::json &pJson) const;

	void serialize(const nlohmann::json &pJson);
};
} // namespace mer::editor::mvp


#endif //PANEDLAYOUTTABS_H
