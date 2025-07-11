//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 23.10.24.
//

#ifndef ILIGHTINSTANCE_H
#define ILIGHTINSTANCE_H

namespace ke {
class LightInstanceData;
class ILightSources;

class ILightInstance {
public:
	virtual ~ILightInstance() = default;

	virtual void notifyDataChanged() = 0;

	[[nodiscard]] virtual ILightSources* getLightSources() const = 0;

	virtual void setLightSources(ILightSources* pLightSources) = 0;

	[[nodiscard]] virtual const LightInstanceData &getLightInstanceData() const = 0;
};
} // namespace ke

#endif //ILIGHTINSTANCE_H
