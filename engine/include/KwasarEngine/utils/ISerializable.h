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
// Created by alexus on 28.10.24.
//

#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H
#include <nlohmann/json_fwd.hpp>

namespace ke {
class Extension;

class ISerializable {
public:
	virtual ~ISerializable() = default;

	virtual void serialize(nlohmann::json &pJson, Extension* pExtension) = 0;

	virtual void deserialize(const nlohmann::json &pJson, Extension* pExtension) = 0;
};
} // namespace ke

#endif //ISERIALIZABLE_H
