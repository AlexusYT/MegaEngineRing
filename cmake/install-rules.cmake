install(
        TARGETS GameEngine_exe
        RUNTIME COMPONENT GameEngine_Runtime
)

if (PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif ()
