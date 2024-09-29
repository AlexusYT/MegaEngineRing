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
// Created by alexus on 25.03.24.
//

#include "EngineSDK/main/scene/SceneDataInjector.h"

#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/MainObjectExtension.h"
#include "EngineSDK/main/scene/objects/extensions/MouseButtonExtension.h"
#include "EngineSDK/main/scene/objects/extensions/cameras/CameraMouseExtension.h"
#include "EngineSDK/main/scene/objects/extensions/cameras/OrbitCameraExtension.h"
#include "EngineSDK/utils/MouseButton.h"

namespace mer::sdk::main {

std::shared_ptr<SceneDataInjector> SceneDataInjector::create(IScene* pScene) {
	return std::make_shared<SceneDataInjector>(pScene);
}

std::shared_ptr<SceneObject> SceneDataInjector::newObject() { return std::make_shared<SceneObject>(); }

void SceneDataInjector::setupEditorCamera(std::shared_ptr<ISceneObject> &pEditorCamera,
										  std::shared_ptr<ICamera> &pCamera) {

	pEditorCamera = newObject();
	auto camera = OrbitCameraExtension::create();
	auto cameraMouse = CameraMouseExtension::create();
	cameraMouse->setEnabled(false);
	auto mouseButton = MouseButtonExtension::create();
	mouseButton->connectButtonSignal(utils::MouseButton::BUTTON_MIDDLE,
									 [cameraMouse](utils::MouseButton /*pButton*/, bool pPressed, double /*pX*/,
												   double /*pY*/) { cameraMouse->setEnabled(pPressed); });

	cameraMouse->propertyAngle.getEvent().connect(camera->propertyAngle.getSetter());
	pEditorCamera->addExtension("cameraMouse", cameraMouse);
	pEditorCamera->addExtension("mouseButton", mouseButton);
	pEditorCamera->addExtension("camera", camera);
	pCamera = camera;
	pEditorCamera->getMainExtension()->propertyName = "EditorCamera";
	//pEditorCamera->setScriptName("EditorCameraScript");
}
} // namespace mer::sdk::main