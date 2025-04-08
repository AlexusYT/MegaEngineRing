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
// Created by alexus on 25.02.25.
//

#ifndef MESH_H
#define MESH_H
#include <sigc++/scoped_connection.h>

#include "EngineUtils/utils/Property.h"

namespace mer::sdk {
class VolumeAabb;
class Primitive;

class Mesh {
	std::vector<std::shared_ptr<Primitive>> primitives;
	std::vector<sigc::scoped_connection> connections;
	Property<std::shared_ptr<VolumeAabb>> aabb;

protected:
	Mesh();

public:
	static std::shared_ptr<Mesh> create();

	[[nodiscard]] PropertyReadOnly<std::shared_ptr<VolumeAabb>> getAabb() { return aabb.getReadOnly(); }

	void addPrimitive(const std::shared_ptr<Primitive> &pPrimitive);

	void setPrimitives(const std::vector<std::shared_ptr<Primitive>> &pPrimitives);

	[[nodiscard]] const std::vector<std::shared_ptr<Primitive>> &getPrimitives() const { return primitives; }

private:
	void buildAabb() const;
};

} // namespace mer::sdk

#endif //MESH_H
