#include <catch2/catch_test_macros.hpp>

#include "lib.h"

TEST_CASE("Name is GameEngine", "[library]") {
	auto const lib = library{};
	REQUIRE(lib.name == "GameEngine");
}
