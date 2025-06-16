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
// Created by alexus on 26.03.24.
//

#include <gtest/gtest.h>

#include "EngineSDK/resources/models/Model3DLoader.h"
#include "EngineSDK/utils/ReportMessage.h"

TEST(ObjModelLoaderTests, ModelLoading) {
	/*using namespace mer::sdk;
	const auto request = std::make_shared<FileModelRequest>("TestName", "main/resources/models/untitled.obj");
	const auto loader = std::make_shared<ObjModelLoader>();
	const std::shared_ptr<Resources> dependencies;
	std::shared_ptr<IResource> resource;
	const auto msg = loader->load(request, dependencies, resource);
	ASSERT_EQ(msg, nullptr) << msg->getReport();
	const auto multiple = std::dynamic_pointer_cast<MultipleResource>(resource);
	ASSERT_NE(multiple, nullptr);
	ASSERT_EQ(multiple->size(), 1);
	const auto iter = multiple->find("Cube");
	ASSERT_NE(iter, multiple->end());
	const auto cube = std::dynamic_pointer_cast<ModelResource>(iter->second);
	ASSERT_NE(cube, nullptr);
	ASSERT_EQ(cube->getVertices().size(), 24);
	ASSERT_EQ(cube->getNormals().size(), 24);
	ASSERT_EQ(cube->getUvs().size(), 24);
	ASSERT_EQ(cube->getIndices().size(), 36);*/
}
