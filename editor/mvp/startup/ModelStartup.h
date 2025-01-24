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
// Created by alexus on 25.01.24.
//

#ifndef MODELSTARTUP_H
#define MODELSTARTUP_H

#include <mvp/startup/IModelStartup.h>

namespace mer::editor::mvp {
class ModelStartup : public IModelStartup {
	std::string name;
	std::filesystem::path path;

public:
	ModelStartup();

	[[nodiscard]] const std::string &getName() const override { return name; }

	void setName(const std::string &pName) override { name = pName; }

	[[nodiscard]] const std::filesystem::path &getPath() const override { return path; }

	void setPath(const std::filesystem::path &pPath) override { path = pPath; }
};
} // namespace mer::editor::mvp

#endif //MODELSTARTUP_H
