cmake_minimum_required(VERSION 3.21)

include(cmake/prelude.cmake)

project(
        MegaEngineRing
        VERSION 0.1.0
        DESCRIPTION "Game engine"
        HOMEPAGE_URL "https://example.com/"
        LANGUAGES CXX
)

set(SDK_VERSION "0.1.0")

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

find_package(PkgConfig REQUIRED)

pkg_check_modules(sigc++-3 REQUIRED IMPORTED_TARGET sigc++-3.0)

find_package(PNG REQUIRED)
message(STATUS "${sigc++-3_LIBRARIES}")
find_package(glm CONFIG REQUIRED)
pkg_check_modules(epoxy REQUIRED IMPORTED_TARGET epoxy)
include(GNUInstallDirs)

add_subdirectory(utils)
add_subdirectory(sdk)
add_subdirectory(editor)


# ---- Declare executable ----

add_executable(MegaEngineEditor source/main.cpp)
add_executable(MegaEngine::Editor ALIAS MegaEngineEditor)
if (LINUX)
    target_link_options(MegaEngineEditor PRIVATE -rdynamic)
endif ()
set_property(TARGET MegaEngineEditor PROPERTY OUTPUT_NAME MegaEngineEditor)

target_link_libraries(MegaEngineEditor PRIVATE MegaEngineEditor_lib)

#[[install(TARGETS MegaEngineEditor
        RUNTIME ARCHIVE LIBRARY RUNTIME FRAMEWORK BUNDLE PUBLIC_HEADER RESOURCE)]]
if (WIN32)

    #[[add_custom_command(TARGET MegaEngineEditor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaEngineEditor> $<TARGET_FILE_DIR:MegaEngineEditor>
            COMMAND_EXPAND_LISTS
    )]]
    install(
            TARGETS MegaEngineEditor
            COMPONENT editor
            RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
            DIRECTORIES $<TARGET_FILE_DIR:MegaEngineEditor>
            $<TARGET_FILE_DIR:MegaEngineSDK_shared>
            "C:\\msys64\\mingw64\\bin"
    )
else ()

    install(
            IMPORTED_RUNTIME_ARTIFACTS MegaEngineEditor
            COMPONENT editor
    )
endif ()

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
