
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/data/RequestSchema.json" REQUEST_SCHEMA_CONTENT)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/RequestSchema.h" "//language=json
const std::string requestSchemaJson = R\"(${REQUEST_SCHEMA_CONTENT})\";")

add_executable(MegaEngineRunner sources/main.cpp
        sources/Client.cpp
        sources/Client.h
        sources/requests/Command.cpp
        sources/requests/Command.h
        sources/Globals.cpp
        sources/Globals.h
        sources/option/CommandParser.cpp
        sources/option/CommandParser.h
        sources/option/CommandOption.cpp
        sources/option/CommandOption.h
        sources/main/options/CommandHostAddress.h
        sources/main/options/CommandHostPort.h
        sources/main/options/CommandHelp.h
        sources/commands/CommandRun.cpp
        sources/commands/CommandRun.h
)
target_include_directories(
        MegaEngineUtils ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_link_libraries(MegaEngineRunner PRIVATE MegaEngineUtils)
target_include_directories(
        MegaEngineRunner ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)
target_include_directories(
        MegaEngineUtils ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/runner/sources>"
)
find_package(asio CONFIG REQUIRED)
target_link_libraries(MegaEngineRunner PRIVATE asio::asio)
find_package(nlohmann_json CONFIG REQUIRED)
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
target_link_libraries(MegaEngineRunner PRIVATE nlohmann_json::nlohmann_json)
find_package(valijson CONFIG REQUIRED)
target_link_libraries(MegaEngineRunner PRIVATE valijson)
find_package(sigc++-3 CONFIG REQUIRED)
target_link_libraries(MegaEngineRunner PRIVATE sigc-3.0)

install(
        IMPORTED_RUNTIME_ARTIFACTS MegaEngineRunner
        COMPONENT editor
)