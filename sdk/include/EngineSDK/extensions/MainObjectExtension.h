//  MegaEngineRing is a program that can speed up game development.
//  Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
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
// Created by alexus on 28.09.24.
//

#ifndef BASICOBJECTEXTENSION_H
#define BASICOBJECTEXTENSION_H
#include <glm/vec3.hpp>

#include "Extension.h"

namespace mer::sdk {

class MainObjectExtension final : public Extension {
	MainObjectExtension();

public:
	METHOD_CREATE(MainObjectExtension)

	EXT_TYPE_NAME("MainObjectExtension")

	ExtensionProperty<std::string> propertyName;
	ExtensionProperty<glm::vec3> propertyPosition;
};

} // namespace mer::sdk

#endif //BASICOBJECTEXTENSION_H
