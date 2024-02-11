# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(GameEngineTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(GameEngineUtils_test source/UUIDTests.cpp)
target_link_libraries(
        GameEngineUtils_test PRIVATE
        GameEngine_utils
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

gtest_discover_tests(GameEngineUtils_test)

# ---- End-of-file commands ----

add_folders(Test)
