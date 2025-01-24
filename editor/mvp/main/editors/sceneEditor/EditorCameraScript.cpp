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
// Created by alexus on 11.10.24.
//

#include "EditorCameraScript.h"

#include "EngineSDK/scene/objects/SceneObject.h"
#include "EngineSDK/extensions/cameras/OrbitCameraExtension.h"

namespace mer::editor::mvp {
sdk::ReportMessagePtr EditorCameraScript::setup() {

	auto extensions = getObject()->getExtensions();
	camera = std::dynamic_pointer_cast<sdk::OrbitCameraExtension>(extensions.at("camera"));
	return Script::setup();
}

bool EditorCameraScript::onMouseScroll(double /*pDx*/, const double pDy) {
	if (!camera) return false;
	auto dy = static_cast<float>(pDy);
	auto distance = camera->propertyDistance.getValue();
	distance += dy * std::log10(distance + 1.1f);
	if (distance < 0.0001f) distance = 0.0001f;

	camera->propertyDistance = distance;
	return true;
}
} // namespace mer::editor::mvp