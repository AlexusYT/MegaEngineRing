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
// Created by alexus on 26.09.24.
//

#ifndef IMODELOBJECTPROPERTIES_H
#define IMODELOBJECTPROPERTIES_H

namespace mer::editor::project {
class LoadedScene;
}

namespace mer::editor::mvp {
class IModelObjectProperties {
public:
	virtual ~IModelObjectProperties() = default;

	[[nodiscard]] virtual const std::shared_ptr<project::LoadedScene> &getLoadedScene() const = 0;

	virtual void setLoadedScene(const std::shared_ptr<project::LoadedScene> &pLoadedScene) = 0;
};
} // namespace mer::editor::mvp


#endif //IMODELOBJECTPROPERTIES_H
