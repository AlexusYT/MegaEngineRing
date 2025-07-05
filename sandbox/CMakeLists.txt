add_library(KwasarEngineSandbox_lib)
target_sources(KwasarEngineSandbox_lib
        PUBLIC
        FILE_SET CXX_MODULES FILES
        Sandbox.cppm
)

target_link_libraries(KwasarEngineSandbox_lib KwasarEngineLib)
add_executable(KwasarEngineSandbox main.cpp)
target_link_libraries(KwasarEngineSandbox KwasarEngineSandbox_lib)
install(TARGETS KwasarEngineSandbox
        DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}
)
