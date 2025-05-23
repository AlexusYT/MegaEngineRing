# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(MegaEngineSDKTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(MegaEngineSDK_test source/main/extensions/CameraExtensionTests.cpp
        source/main/resources/models/ObjModelLoaderTests.cpp
        source/main/prefabs/PrefabTests.cpp
        source/utils/TransformationTests.cpp
)
target_link_libraries(MegaEngineSDK_test PRIVATE MegaEngineSDK_shared MegaEngineUtils)
target_link_libraries(
        MegaEngineSDK_test PRIVATE
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

#[[add_custom_command(TARGET MegaEngineSDK_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaEngineSDK_test> $<TARGET_FILE_DIR:MegaEngineSDK_test>

        COMMAND_EXPAND_LISTS
)]]
#gtest_discover_tests(MegaEngineSDK_test)
add_custom_command(
        TARGET MegaEngineSDK_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data/
        ${CMAKE_CURRENT_BINARY_DIR}/)


# ---- End-of-file commands ----

add_folders(Test)
