# ---- Developer mode ----

# Developer mode enables targets and code paths in the CMake scripts that are
# only relevant for the developer(s) of MegaEngineRing
# Targets necessary to build the project must be provided unconditionally, so
# consumers can trivially build and package the project
if (PROJECT_IS_TOP_LEVEL)
    option(MegaEngine_DEVELOPER_MODE "Enable developer mode" OFF)
endif ()

# ---- Warning guard ----

# target_include_directories with the SYSTEM modifier will request the compiler
# to omit warnings from the provided paths, if the compiler supports that
# This is to provide a user experience similar to find_package when
# add_subdirectory or FetchContent is used to consume this project
set(warning_guard "")
if (NOT PROJECT_IS_TOP_LEVEL)
    option(
            MegaEngine_INCLUDES_WITH_SYSTEM
            "Use SYSTEM modifier for MegaEngine's includes, disabling warnings"
            ON
    )
    mark_as_advanced(MegaEngine_INCLUDES_WITH_SYSTEM)
    if (MegaEngine_INCLUDES_WITH_SYSTEM)
        set(warning_guard SYSTEM)
    endif ()
endif ()
