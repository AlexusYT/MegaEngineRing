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

#include "EngineSDK/main/resources/materials/ColorComponent.h"

namespace mer::sdk::main {
ColorComponent::ColorComponent(const glm::vec4 &pColor) : color(nullptr, "ColorComponent") { color = pColor; }

std::shared_ptr<ColorComponent> ColorComponent::create() { return create({0.0f, 0.0f, 0.0f, 1.0f}); }

std::shared_ptr<ColorComponent> ColorComponent::create(const glm::vec4 &pColor) {
	return std::shared_ptr<ColorComponent>(new ColorComponent(pColor));
}

std::shared_ptr<ColorComponent> ColorComponent::create(const float pRed, const float pGreen, const float pBlue,
													   const float pAlpha) {
	return create(glm::vec4(pRed, pGreen, pBlue, pAlpha));
}

} // namespace mer::sdk::main