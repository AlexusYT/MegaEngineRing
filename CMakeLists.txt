cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
        MegaEngineRing
        VERSION 0.1.0
        DESCRIPTION "Game engine"
        HOMEPAGE_URL "https://example.com/"
        LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

find_package(PkgConfig REQUIRED)

add_subdirectory(utils)
add_subdirectory(sdk)
add_subdirectory(editor)


# ---- Declare executable ----

add_executable(MegaEngineEditor source/main.cpp)
add_executable(MegaEngine::Editor ALIAS MegaEngineEditor)
target_link_options(MegaEngineEditor PRIVATE -rdynamic)

set_property(TARGET MegaEngineEditor PROPERTY OUTPUT_NAME MegaEngineEditor)

target_link_libraries(MegaEngineEditor PRIVATE MegaEngineEditor_lib)
# Remove existing Resources directory before copying and copy new resources
add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
        VERBATIM)

add_dependencies(MegaEngineEditor CopyResources)

# ---- Install rules ----

if (NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif ()

# ---- Developer mode ----

if (NOT MegaEngine_DEVELOPER_MODE)
    return()
elseif (NOT PROJECT_IS_TOP_LEVEL)
    message(
            AUTHOR_WARNING
            "Developer mode is intended for developers of MegaEngineRing"
    )
endif ()

include(cmake/dev-mode.cmake)
