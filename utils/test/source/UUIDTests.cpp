#include <gtest/gtest.h>
#include <ranges>

#include "EngineUtils/utils/UUID.h"

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
	mer::sdk::utils::ReportMessagePtr msg;
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
	mer::sdk::utils::ReportMessagePtr msg;
	auto uuid = UUID::parse("A27C75DB-3886-17B2-F65A-C3B4D89D24F2sdfsd", msg);
	EXPECT_EQ(uuid, nullptr);
	ASSERT_NE(msg, nullptr);
	EXPECT_EQ(msg->getMessage(), "The string doesn't match the regex");
}