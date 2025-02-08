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
find_package(glm CONFIG REQUIRED)
pkg_check_modules(epoxy REQUIRED IMPORTED_TARGET epoxy)
include(GNUInstallDirs)

#
# Applies CMAKE_CXX_FLAGS to all targets in the current CMake directory.
# After this operation, CMAKE_CXX_FLAGS is cleared.
#
macro(apply_global_cxx_flags_to_all_targets)
    separate_arguments(_global_cxx_flags_list UNIX_COMMAND ${CMAKE_CXX_FLAGS})
    get_property(_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
    foreach (_target ${_targets})
        target_compile_options(${_target} PUBLIC ${_global_cxx_flags_list})
    endforeach ()
    unset(CMAKE_CXX_FLAGS)
    set(_flag_sync_required TRUE)
endmacro()

#
# Removes the specified compile flag from the specified target.
#   _target     - The target to remove the compile flag from
#   _flag       - The compile flag to remove
#
# Pre: apply_global_cxx_flags_to_all_targets() must be invoked.
#
macro(remove_flag_from_target _target _flag)
    get_target_property(_target_cxx_flags ${_target} COMPILE_OPTIONS)
    if (_target_cxx_flags)
        list(REMOVE_ITEM _target_cxx_flags ${_flag})
        set_target_properties(${_target} PROPERTIES COMPILE_OPTIONS "${_target_cxx_flags}")
    endif ()
endmacro()

add_library(ImGUI STATIC
        submodules/imgui/imgui.cpp
        submodules/imgui/imgui.h
        submodules/imgui/imgui_internal.h
        submodules/imgui/imgui_draw.cpp
        submodules/imgui/imgui_demo.cpp
        submodules/imgui/imgui_tables.cpp
        submodules/imgui/imgui_widgets.cpp
        submodules/imgui/backends/imgui_impl_glfw.cpp
        submodules/imgui/backends/imgui_impl_opengl3.cpp)
target_compile_options(ImGUI BEFORE PRIVATE -w)
target_include_directories(ImGUI SYSTEM PUBLIC "submodules/imgui")
target_include_directories(ImGUI SYSTEM PUBLIC "submodules/imgui/backends")

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

apply_global_cxx_flags_to_all_targets()
remove_flag_from_target(ImGUI -Werror)
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
