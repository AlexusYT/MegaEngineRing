

set(ENGINE_SOURCES sources/main/Application.cpp
        sources/main/DefaultApplicationSettings.cpp
)
set(ENGINE_HEADERS include/EngineSDK/main/Application.h
        include/EngineSDK/main/IApplication.h
        include/EngineSDK/main/context/IContext.h
        include/EngineSDK/main/IWindow.h
        include/EngineSDK/main/IApplicationSettings.h
        include/EngineSDK/main/DefaultApplicationSettings.h
        include/EngineSDK/utils/Property.h)

add_library(GameEngine_engine_shared SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})
add_library(GameEngine_engine_static STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_sources(GameEngine_engine_shared PUBLIC FILE_SET engine_headers TYPE HEADERS FILES ${ENGINE_HEADERS})

target_precompile_headers(GameEngine_engine_shared
        PRIVATE common.h
)
target_precompile_headers(GameEngine_engine_static
        PRIVATE common.h
)

target_include_directories(
        GameEngine_engine_shared ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/engine/include>"
)
target_include_directories(
        GameEngine_engine_shared ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)
target_include_directories(
        GameEngine_engine_static ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/engine/include>"
)
target_include_directories(
        GameEngine_engine_static ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)
find_package(sigc++-3 CONFIG REQUIRED)

target_compile_features(GameEngine_engine_shared PUBLIC cxx_std_23)
target_link_libraries(GameEngine_engine_shared PUBLIC stdc++exp GameEngine_utils)
target_link_libraries(GameEngine_engine_shared PRIVATE sigc-3.0)
set_property(TARGET GameEngine_engine_shared PROPERTY OUTPUT_NAME GameEngine)

target_compile_features(GameEngine_engine_static PUBLIC cxx_std_23)
target_link_libraries(GameEngine_engine_static PUBLIC stdc++exp GameEngine_utils)
target_link_libraries(GameEngine_engine_static PRIVATE sigc-3.0)
set_property(TARGET GameEngine_engine_static PROPERTY OUTPUT_NAME GameEngine)

install(
        TARGETS GameEngine_engine_static GameEngine_engine_shared
        ARCHIVE
        DESTINATION sdk/1.0.0/lib/
        COMPONENT sdk-1.0.0
        LIBRARY
        DESTINATION sdk/1.0.0/lib/
        COMPONENT sdk-1.0.0
        FILE_SET engine_headers
        DESTINATION sdk/1.0.0/
        COMPONENT sdk-1.0.0
)


