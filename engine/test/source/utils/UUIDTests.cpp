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

#include <gtest/gtest.h>
#include <ranges>

#include "KwasarEngine/utils/UUID.h"
#include "KwasarEngine/utils/ReportMessage.h"

using namespace ke;

TEST(UUID_TESTS, ZeroUUIDToString) {
	constexpr UUID uuid;
	EXPECT_EQ(uuid.toString(false), "00000000-0000-0000-0000-000000000000");
	EXPECT_EQ(uuid.toString(true), "00000000-0000-0000-0000-000000000000");
}

TEST(UUID_TESTS, RandomUUID) {
	auto uuid1 = UUID::create();
	auto uuid2 = UUID::create();
	EXPECT_NE(uuid1->toString(), uuid2->toString());
}

TEST(UUID_TESTS, RandomUUIDUpper) {
	auto uuid1 = UUID::create();
	EXPECT_EQ(uuid1->toString(true), uuid1->toString(false) | std::ranges::views::transform([](char ch) {
				  return std::toupper(ch);
				  }) | std::ranges::to<std::string>());
}

TEST(UUID_TESTS, ParseUUID) {
	ke::ReportMessagePtr msg;
	auto uuid = UUID::parse("A27C75DB-3886-17B2-F65A-C3B4D89D24F2", msg);
	ASSERT_NE(uuid, nullptr) << (msg ? msg->getReport() : "");
	EXPECT_EQ(uuid->toString(true), "A27C75DB-3886-17B2-F65A-C3B4D89D24F2");
	EXPECT_EQ(uuid->toString(false), "a27c75db-3886-17b2-f65a-c3b4d89d24f2");

	uuid = UUID::parse("a27c75db-3886-17b2-f65a-c3b4d89d24f2", msg);
	ASSERT_NE(uuid, nullptr) << (msg ? msg->getReport() : "");
	EXPECT_EQ(uuid->toString(true), "A27C75DB-3886-17B2-F65A-C3B4D89D24F2");
	EXPECT_EQ(uuid->toString(false), "a27c75db-3886-17b2-f65a-c3b4d89d24f2");
}

TEST(UUID_TESTS, ParseUUIDNotMatch) {
	ke::ReportMessagePtr msg;
	auto uuid = UUID::parse("A27C75DB-3886-17B2-F65A-C3B4D89D24F2sdfsd", msg);
	EXPECT_EQ(uuid, nullptr);
	ASSERT_NE(msg, nullptr);
	EXPECT_EQ(msg->getMessage(), "The string doesn't match the regex");
}
