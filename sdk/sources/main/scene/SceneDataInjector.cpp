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
#include "EngineSDK/main/scene/objects/extensions/cameras/CameraMouseExtension.h"
#include "EngineSDK/main/scene/objects/extensions/cameras/OrbitCameraExtension.h"

namespace mer::sdk::main {

std::shared_ptr<SceneObject> SceneDataInjector::newObject() { return std::make_shared<SceneObject>(); }

std::shared_ptr<ISceneObject> SceneDataInjector::injectEditorCamera() {

	std::shared_ptr<ISceneObject> editorCamera = newObject();
	auto camera = OrbitCameraExtension::create();
	auto cameraMouse = CameraMouseExtension::create();
	cameraMouse->setMethodAddAngle(sigc::mem_fun(*camera, &OrbitCameraExtension::addAngle));
	scene->switchCamera(camera.get());
	camera->getOnMatrixChanged().connect(sigc::mem_fun(*scene, &IScene::setViewProjMatrix));
	editorCamera->addExtension("cameraMouse", cameraMouse);
	editorCamera->addExtension("camera", camera);
	editorCamera->setName("EditorCamera");
	scene->addObject(editorCamera);
	return editorCamera;
}
} // namespace mer::sdk::main