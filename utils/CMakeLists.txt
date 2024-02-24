

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

add_library(MegaEngineUtils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})
target_sources(MegaEngineUtils PUBLIC FILE_SET utils_headers TYPE HEADERS FILES ${UTILS_HEADERS})

target_include_directories(
        MegaEngineUtils ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)

target_compile_features(MegaEngineUtils PUBLIC cxx_std_23)
target_link_libraries(MegaEngineUtils PRIVATE stdc++exp)
target_compile_options(MegaEngineUtils PRIVATE -fPIC)
set_property(TARGET MegaEngineUtils PROPERTY OUTPUT_NAME MegaEngineUtils)
set_property(TARGET MegaEngineUtils PROPERTY PREFIX "")

install(
        TARGETS MegaEngineUtils
        ARCHIVE
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
        COMPONENT sdk
        FILE_SET utils_headers
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/"
        COMPONENT sdk
)

