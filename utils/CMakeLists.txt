

set(UTILS_SOURCES
        sources/utils/Logger.cpp
        sources/utils/ReportMessage.cpp
        sources/utils/ConsoleColors.cpp
        sources/utils/Utils.cpp
        sources/utils/UUID.cpp
)
set(UTILS_HEADERS
        include/EngineUtils/utils/Logger.h
        include/EngineUtils/utils/ReportMessage.h
        include/EngineUtils/utils/ReportMessageFwd.h
        include/EngineUtils/utils/Utils.h
        include/EngineUtils/utils/UUID.h
)

add_library(GameEngine_utils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})
target_sources(GameEngine_utils PUBLIC FILE_SET utils_headers TYPE HEADERS FILES ${UTILS_HEADERS})

target_include_directories(
        GameEngine_utils ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)

target_compile_features(GameEngine_utils PUBLIC cxx_std_23)
target_link_libraries(GameEngine_utils PRIVATE stdc++exp)
target_compile_options(GameEngine_utils PRIVATE -fPIC)
set_property(TARGET GameEngine_utils PROPERTY OUTPUT_NAME GameEngineUtils)

install(
        TARGETS GameEngine_utils
        ARCHIVE
        DESTINATION sdk/1.0.0/lib
        COMPONENT sdk-1.0.0
        FILE_SET utils_headers
        DESTINATION sdk/1.0.0/
        COMPONENT sdk-1.0.0
)

