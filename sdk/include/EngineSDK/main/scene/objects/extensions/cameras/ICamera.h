// MegaEngineRing is a program that can speed up game development.
// Copyright (C) 2024. Timofeev (Alexus_XX) Alexander
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

//
// Created by alexus on 24.02.24.
//

#ifndef ICAMERA_H
#define ICAMERA_H
#include <glm/fwd.hpp>
#include <sigc++/signal.h>

namespace mer::sdk::main {
template<typename T>
class ExtensionProperty;

class ICamera {
public:
	virtual ~ICamera() = default;


	[[nodiscard]] virtual sigc::signal<void(const glm::mat4 &)> &getOnMatrixChanged() = 0;

	[[nodiscard]] virtual const glm::mat4 &getMatrix() const = 0;

	[[nodiscard]] virtual ExtensionProperty<glm::vec3> &getPosition() = 0;

	virtual void updateMatrix() = 0;
};
} // namespace mer::sdk::main

#endif //ICAMERA_H
