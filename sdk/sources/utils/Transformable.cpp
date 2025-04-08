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

#include "EngineSDK/utils/Transformable.h"

#include "EngineSDK/utils/Transformation.h"

namespace mer::sdk {
Transformable::Transformable() : globalTransform(Transformation::create()), localTransform(Transformation::create()) {
	localTransform->connectOnChanged([this](const glm::mat4 & /*pNewMatrix*/) {
		onLocalTransformChanged(localTransform);
		updateTransform();
	});
	globalTransform->connectOnChanged(
		[this](const glm::mat4 & /*pNewMatrix*/) { onGlobalTransformChanged(globalTransform); });
}

void Transformable::updateTransform() {

	std::shared_ptr<Transformation> transform;
	if (parent) transform = parent->globalTransform->clone();
	else
		transform = Transformation::create();
	transform->applyTransformation(localTransform);
	globalTransform->swapTransformation(transform);
}

void Transformable::onLocalTransformChanged(const std::shared_ptr<Transformation> & /*pTransformation*/) {}

void Transformable::onGlobalTransformChanged(const std::shared_ptr<Transformation> & /*pTransformation*/) {}
} // namespace mer::sdk