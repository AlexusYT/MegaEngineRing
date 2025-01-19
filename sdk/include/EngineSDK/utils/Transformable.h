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
// Created by alexus on 05.12.24.
//

#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H
#include <memory>

namespace mer::sdk::utils {
class Transformation;
}

namespace mer::sdk::main {

class Transformable {
	Transformable* parent{};
	std::shared_ptr<utils::Transformation> globalTransform;
	std::shared_ptr<utils::Transformation> localTransform;

public:
	Transformable();

	virtual ~Transformable() = default;

	[[nodiscard]] const std::shared_ptr<utils::Transformation> &getGlobalTransform() const { return globalTransform; }

	[[nodiscard]] const std::shared_ptr<utils::Transformation> &getLocalTransform() { return localTransform; }

	[[nodiscard]] Transformable* getTransformParent() const { return parent; }

	void setTransformParent(Transformable* pParent) {
		if (parent == pParent) return;
		parent = pParent;
		updateTransform();
	}

	virtual void updateTransform();

protected:
	virtual void onLocalTransformChanged(const std::shared_ptr<utils::Transformation> &pTransformation);

	virtual void onGlobalTransformChanged(const std::shared_ptr<utils::Transformation> &pTransformation);
};

} // namespace mer::sdk::main

#endif //TRANSFORMABLE_H
