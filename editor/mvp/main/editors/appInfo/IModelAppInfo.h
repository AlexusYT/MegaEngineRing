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
// Created by alexus on 29.01.24.
//

#ifndef IMODELAPPINFO_H
#define IMODELAPPINFO_H

namespace mer::editor::mvp {
class IModelAppInfo {
public:
	virtual ~IModelAppInfo() = default;

	[[nodiscard]] virtual project::ApplicationInfo* getInfo() const = 0;

	virtual void setInfo(project::ApplicationInfo* pInfo) = 0;

	[[nodiscard]] virtual const std::shared_ptr<project::Project> &getProject() const = 0;

	virtual void setProject(const std::shared_ptr<project::Project> &pProject) = 0;

};
} // namespace mer::editor::mvp

#endif //IMODELAPPINFO_H
