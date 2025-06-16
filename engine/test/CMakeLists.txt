# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(MegaKwasarEngineTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# ---- Tests ----

add_executable(MegaKwasarEngine_test source/main/extensions/CameraExtensionTests.cpp
        source/main/resources/models/ObjModelLoaderTests.cpp
        source/main/prefabs/PrefabTests.cpp
        source/utils/TransformationTests.cpp
        source/utils/LoggerTests.cpp
        source/utils/UUIDTests.cpp
)
target_link_libraries(MegaKwasarEngine_test PRIVATE KwasarEngineLib)
target_link_libraries(
        MegaKwasarEngine_test PRIVATE
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

#[[add_custom_command(TARGET MegaKwasarEngine_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaKwasarEngine_test> $<TARGET_FILE_DIR:MegaKwasarEngine_test>

        COMMAND_EXPAND_LISTS
)]]
#gtest_discover_tests(MegaKwasarEngine_test)
add_custom_command(
        TARGET MegaKwasarEngine_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data/
        ${CMAKE_CURRENT_BINARY_DIR}/)


# ---- End-of-file commands ----

add_folders(Test)
