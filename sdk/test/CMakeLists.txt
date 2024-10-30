# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(MegaEngineSDKTests LANGUAGES CXX)

# ---- Dependencies ----

find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(MegaEngineSDK_test source/main/scene/objects/extensions/CameraExtensionTests.cpp
        source/main/resources/models/ObjModelLoaderTests.cpp)
target_link_libraries(MegaEngineSDK_test PRIVATE MegaEngineSDK_shared)
target_include_directories(MegaEngineSDK_shared PRIVATE ${PNGPP_INCLUDE_DIRS})
find_package(PNG REQUIRED)
target_link_libraries(MegaEngineSDK_shared PRIVATE PNG::PNG)
target_link_libraries(
        MegaEngineSDK_test PRIVATE
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
pkg_check_modules(epoxy REQUIRED IMPORTED_TARGET epoxy)
target_link_libraries(MegaEngineSDK_test PRIVATE PkgConfig::epoxy)

gtest_discover_tests(MegaEngineSDK_test)
add_custom_command(
        TARGET MegaEngineSDK_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data/
        ${CMAKE_CURRENT_BINARY_DIR}/)


# ---- End-of-file commands ----

add_folders(Test)
