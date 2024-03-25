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

#ifndef SCENEDATAINJECTOR_H
#define SCENEDATAINJECTOR_H
#include <memory>

#include "ISceneDataInjector.h"
#include "objects/SceneObject.h"

namespace mer::sdk::main {
class SceneDataInjector : public ISceneDataInjector {
	std::shared_ptr<IScene> scene;

public:
	explicit SceneDataInjector(IScene* pScene) : scene(pScene) {}

	[[nodiscard]] const std::shared_ptr<IScene> &getScene() const override { return scene; }

	std::shared_ptr<SceneObject> newObject() override;

	std::shared_ptr<ISceneObject> injectEditorCamera() override;
};
} // namespace mer::sdk::main


#endif //SCENEDATAINJECTOR_H
