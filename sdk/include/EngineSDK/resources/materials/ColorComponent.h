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
// Created by alexus on 23.11.24.
//

#ifndef COLORCOMPONENT_H
#define COLORCOMPONENT_H
#include <glm/vec4.hpp>

#include "IMaterialComponent.h"

namespace mer::sdk {
class ColorComponent : public IMaterialComponent {
	ColorComponent(const glm::vec4 &pColor);

public:
	static std::shared_ptr<ColorComponent> create();

	static std::shared_ptr<ColorComponent> create(const glm::vec4 &pColor);

	static std::shared_ptr<ColorComponent> create(float pRed, float pGreen, float pBlue, float pAlpha = 1.0f);

	Property<glm::vec4> color;

private:
	PropertyReadOnly<glm::vec4> getComponentValueProperty() override { return color.getReadOnly(); }
};
} // namespace mer::sdk

#endif //COLORCOMPONENT_H
