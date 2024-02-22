# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(MegaEngineUtilsTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(MegaEngineUtils_test source/UUIDTests.cpp
        source/UtilsTests.cpp
        source/LoggerTests.cpp)
target_link_libraries(
        MegaEngineUtils_test PRIVATE
        MegaEngineUtils
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

gtest_discover_tests(MegaEngineUtils_test)

# ---- End-of-file commands ----

add_folders(Test)
