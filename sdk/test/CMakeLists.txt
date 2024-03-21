# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(MegaEngineSDKTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(MegaEngineSDK_test source/main/scene/objects/extensions/CameraExtensionTests.cpp)
target_link_libraries(
        MegaEngineSDK_test PRIVATE
        MegaEngineSDK_static
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
pkg_check_modules(epoxy REQUIRED IMPORTED_TARGET epoxy)
target_link_libraries(MegaEngineSDK_test PRIVATE PkgConfig::epoxy)

gtest_discover_tests(MegaEngineSDK_test)

# ---- End-of-file commands ----

add_folders(Test)
