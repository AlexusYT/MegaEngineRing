//  KwasarEngine is an SDK that can help you speed up game development.
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
// Created by alexus on 04.12.24.
//

#include <gtest/gtest.h>

#include "KwasarEngine/utils/Transformation.h"
using namespace ke;

TEST(TransformationTests, getPosition) {
	{
		auto transform = Transformation::create();
		transform->translate(1.0f, 2.0f, 3.0f);
		transform->scale(1.0f, 4.0f, 5.0f);
		EXPECT_EQ(transform->getPosition().x, 1.0f);
		EXPECT_EQ(transform->getPosition().y, 2.0f);
		EXPECT_EQ(transform->getPosition().z, 3.0f);
	}
	{
		auto transform = Transformation::create();
		transform->scale(1.0f, 4.0f, 5.0f);
		transform->translate(1.0f, 2.0f, 3.0f);
		EXPECT_EQ(transform->getPosition().x, 1.0f);
		EXPECT_EQ(transform->getPosition().y, 8.0f);
		EXPECT_EQ(transform->getPosition().z, 15.0f);
	}
}

TEST(TransformationTests, Copying) {
	auto transform = Transformation::create();
	transform->translate(1.0f, 2.0f, 3.0f);
	transform->scale(1.0f, 4.0f, 5.0f);

	auto transform1 = transform->clone();
	transform1->translate(2.0f, 2.0f, 2.0f);

	EXPECT_EQ(transform1->getPosition().x, 3.0f);
	EXPECT_EQ(transform1->getPosition().y, 10.0f);
	EXPECT_EQ(transform1->getPosition().z, 13.0f);

	EXPECT_EQ(transform->getPosition().x, 1.0f);
	EXPECT_EQ(transform->getPosition().y, 2.0f);
	EXPECT_EQ(transform->getPosition().z, 3.0f);
}