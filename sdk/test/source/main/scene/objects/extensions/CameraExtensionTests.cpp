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
// Created by alexus on 29.02.24.
//

#include <gtest/gtest.h>

#include "EngineSDK/main/scene/objects/SceneObject.h"
#include "EngineSDK/main/scene/objects/extensions/CameraExtension.h"
#include "EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h"

using namespace mer::sdk::main;

//TODO Tests
class CameraExtensionTestsFixture : public testing::Test {
protected:
	std::shared_ptr<SceneObject> obj;
	std::shared_ptr<CameraExtension> ext;

	static void SetUpTestSuite() { ExtensionRegistry::registerClass<CameraExtension>(); }

	static void TearDownTestSuite() {}

	void SetUp() override {
		obj = std::make_shared<SceneObject>();
		ext = CameraExtension::create();
		obj->addExtension("test-ext", ext);
		auto iObj = std::dynamic_pointer_cast<ISceneObject>(obj);
		iObj->init();
	}

	void TearDown() override {}
};

TEST_F(CameraExtensionTestsFixture, MatrixUpdate) {
	auto mat = ext->getMatrix();
	ext->setAngle({0.2f, 20.5f});
	EXPECT_NE(mat, ext->getMatrix());
}