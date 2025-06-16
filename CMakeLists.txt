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
include(cmake/gettext-helper.cmake)


# ---- Declare library ----

find_package(PkgConfig REQUIRED)

pkg_check_modules(sigc++-3 REQUIRED IMPORTED_TARGET sigc++-3.0)
pkg_check_modules(libcurl REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(libturbojpeg REQUIRED IMPORTED_TARGET libturbojpeg)
if (WIN32)
    pkg_check_modules(libspng REQUIRED IMPORTED_TARGET libspng)
else ()
    pkg_check_modules(spng REQUIRED IMPORTED_TARGET spng)
endif ()
find_package(glm CONFIG REQUIRED)
find_package(Gettext REQUIRED)
pkg_check_modules(epoxy REQUIRED IMPORTED_TARGET epoxy)
include(GNUInstallDirs)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(ENABLE_UNIT_TESTS OFF)
set(ENABLE_SAMPLES OFF)
add_subdirectory(submodules/glTF-SDK)
target_include_directories(GLTFSDK SYSTEM PUBLIC "submodules/glTF-SDK/GLTFSDK/Inc/")
target_compile_options(GLTFSDK PRIVATE -fPIC)

add_library(ImGUI OBJECT
        submodules/imgui/imgui.cpp
        submodules/imgui/imgui.h
        submodules/imgui/imgui_internal.h
        submodules/imgui/imgui_draw.cpp
        submodules/imgui/imgui_demo.cpp
        submodules/imgui/imgui_tables.cpp
        submodules/imgui/imgui_widgets.cpp
        submodules/imgui/backends/imgui_impl_glfw.cpp
        submodules/imgui/backends/imgui_impl_opengl3.cpp)
target_compile_options(ImGUI PRIVATE -fPIC)
target_compile_options(ImGUI BEFORE PRIVATE -w)
target_include_directories(ImGUI SYSTEM PUBLIC "submodules/imgui")
target_include_directories(ImGUI SYSTEM PUBLIC "submodules/imgui/backends")
target_link_libraries(ImGUI PUBLIC glm::glm)

execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_SOURCE_DIR}/submodules/imgui/misc/fonts/binary_to_compressed_c.cpp -o ImGuiB2C
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE RESULT_B2C_COMPILATION
        ERROR_VARIABLE RESULT_B2C_COMPILATION_ERROR
)
if (NOT ${RESULT_B2C_COMPILATION} EQUAL 0)
    message(FATAL_ERROR "Failed to compile ImGuiB2C utility:
    Compiler: ${CMAKE_CXX_COMPILER}
    ${RESULT_B2C_COMPILATION_ERROR}
    The utility is used to compress the TTF files and save it as C-array. It is required to run, so, try to fix the error
    ")
else ()
    message(STATUS "Compilation of ImGuiB2C utility succeeded")
endif ()

target_compile_definitions(ImGUI PUBLIC "IMGUI_USER_CONFIG=\"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/imconfig_custom.h>\"")

add_subdirectory(submodules/ImGuiFileDialog)
target_link_libraries(ImGuiFileDialog PRIVATE ImGUI)
target_compile_definitions(ImGuiFileDialog PUBLIC "CUSTOM_IMGUIFILEDIALOG_CONFIG=\"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/editor/ImGuiFileDialogConfigCustom.h>\"")


add_subdirectory(sdk)
add_subdirectory(editor)

# ---- Declare executable ----

if (LINUX)
    target_link_options(MegaEngineEditor PRIVATE -rdynamic)
endif ()
add_executable(KwasarEngineEditor source/main.cpp)
add_executable(KwasarEngine::Editor ALIAS KwasarEngineEditor)
set_property(TARGET KwasarEngineEditor PROPERTY OUTPUT_NAME KwasarEngineEditor)

target_link_libraries(KwasarEngineEditor PRIVATE KwasarEngineEditor_lib)
target_compile_options(KwasarEngineEditor PRIVATE -Werror)

if (WIN32)
    install(
            TARGETS KwasarEngineEditor
            COMPONENT editor
            RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
            DIRECTORIES $<TARGET_FILE_DIR:KwasarEngineEditor>
            $<TARGET_FILE_DIR:KwasarEngineLib>
            $ENV{PATH}
    )
else ()

    install(
            IMPORTED_RUNTIME_ARTIFACTS KwasarEngineEditor
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
            "Developer mode is intended for developers of KwasarEngine"
    )
endif ()

include(cmake/dev-mode.cmake)
