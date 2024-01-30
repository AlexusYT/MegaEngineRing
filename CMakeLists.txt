cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
        GameEngine
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
add_subdirectory(engine)
add_subdirectory(core)


# ---- Declare executable ----

add_executable(GameEngine_UI source/main.cpp)
add_executable(GameEngine::UI ALIAS GameEngine_UI)
target_link_options(GameEngine_UI PRIVATE -rdynamic)

set_property(TARGET GameEngine_UI PROPERTY OUTPUT_NAME GameEngineUI)

target_compile_features(GameEngine_UI PRIVATE cxx_std_23)

target_link_libraries(GameEngine_UI PRIVATE GameEngine_core)
# Remove existing Resources directory before copying and copy new resources
add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
        VERBATIM)

add_dependencies(GameEngine_UI CopyResources)
add_custom_target(BuildAll ALL DEPENDS GameEngine_UI GameEngine_engine_shared GameEngine_engine_static)

# ---- Install rules ----

if (NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif ()

# ---- Developer mode ----

if (NOT GameEngine_DEVELOPER_MODE)
    return()
elseif (NOT PROJECT_IS_TOP_LEVEL)
    message(
            AUTHOR_WARNING
            "Developer mode is intended for developers of GameEngine"
    )
endif ()

include(cmake/dev-mode.cmake)
