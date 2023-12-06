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


add_library(
        GameEngine_lib OBJECT
        source/lib.cpp
        source/GameEngine.cpp
        source/MainWindow.cpp
        source/graphic/viewport/OpenGLRender.cpp
        source/graphic/viewport/OpenGLRender.h
        source/graphic/viewport/OpenGLRenderFwd.h
        source/graphic/viewport/Camera.cpp
        source/graphic/viewport/Camera.h
        source/graphic/viewport/CameraFwd.h

)

target_precompile_headers(GameEngine_lib
        PRIVATE source/common.h
)

target_include_directories(
        GameEngine_lib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(GameEngine_lib PUBLIC cxx_std_20)

find_package(ZLIB REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE ZLIB::ZLIB)

# Each component imports a target:
# TARGETS: PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX
find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX)

pkg_check_modules(LIBFFI REQUIRED IMPORTED_TARGET libffi)
target_link_libraries(GameEngine_lib PRIVATE PkgConfig::LIBFFI)

find_package(unofficial-libffi CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE unofficial::libffi::libffi)

find_package(Intl REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE Intl::Intl) # since CMake 3.20

find_package(unofficial-brotli CONFIG REQUIRED)
# Brotli decoder library
target_link_libraries(GameEngine_lib PRIVATE unofficial::brotli::brotlidec)
# Brotli encoder library
target_link_libraries(GameEngine_lib PRIVATE unofficial::brotli::brotlienc)

find_package(BZip2 REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE BZip2::BZip2)

find_package(PNG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE PNG::PNG)

find_package(Freetype REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE Freetype::Freetype) # since CMake 3.10

# this is heuristically generated, and may not be correct
find_package(unofficial-libuuid CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE unofficial::UUID::uuid)

# this is heuristically generated, and may not be correct
find_package(expat CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE expat::expat)

find_package(Fontconfig REQUIRED) # since CMake 3.14
target_link_libraries(GameEngine_lib PRIVATE Fontconfig::Fontconfig)

# cairo
pkg_check_modules(cairo REQUIRED IMPORTED_TARGET cairo)
target_link_libraries(GameEngine_lib PRIVATE PkgConfig::cairo)

# cairo-script-interpreter
pkg_check_modules(cairo-script-interpreter REQUIRED IMPORTED_TARGET cairo-script-interpreter)
target_link_libraries(GameEngine_lib PRIVATE PkgConfig::cairo-script-interpreter)

# this is heuristically generated, and may not be correct
find_package(sigc++-3 CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE sigc-3.0)

pkg_check_modules(CAIROMM REQUIRED IMPORTED_TARGET cairomm-1.16)
target_link_libraries(GameEngine_lib PRIVATE PkgConfig::CAIROMM)

find_path(EGL_REGISTRY_INCLUDE_DIRS "EGL/egl.h")
target_include_directories(GameEngine_lib PRIVATE ${EGL_REGISTRY_INCLUDE_DIRS})

find_package(fmt REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE fmt::fmt)

find_package(LibLZMA REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE LibLZMA::LibLZMA)

# this is heuristically generated, and may not be correct
find_package(harfbuzz CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE harfbuzz::harfbuzz)

# this is heuristically generated, and may not be correct
find_package(unofficial-pcre CONFIG REQUIRED)
# note: 1 additional targets are not displayed.
target_link_libraries(GameEngine_lib PRIVATE unofficial::pcre::pcre unofficial::pcre::pcre16 unofficial::pcre::pcre32 unofficial::pcre::pcrecpp)

find_package(JPEG REQUIRED)
target_include_directories(GameEngine_lib PRIVATE JPEG::JPEG)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE glfw)


find_package(GLEW REQUIRED)
target_link_libraries(GameEngine_lib PRIVATE GLEW::GLEW)

pkg_check_modules(GTKMM gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(GameEngine_lib PRIVATE ${GTKMM_LINK_LIBRARIES})
message(STATUS ${GTKMM_LINK_LIBRARIES})

# ---- Declare executable ----

add_executable(GameEngine_exe source/main.cpp)
add_executable(GameEngine::exe ALIAS GameEngine_exe)
target_link_options(GameEngine_exe PRIVATE -rdynamic)

set_property(TARGET GameEngine_exe PROPERTY OUTPUT_NAME GameEngine)

target_compile_features(GameEngine_exe PRIVATE cxx_std_23)

target_link_libraries(GameEngine_exe PRIVATE GameEngine_lib)
# Remove existing Resources directory before copying and copy new resources
add_custom_target(CopyResources ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources
        VERBATIM)

add_dependencies(GameEngine_exe CopyResources)

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
