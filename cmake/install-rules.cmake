#[[install(
        TARGETS GameEngine_UI
        RUNTIME COMPONENT GameEngine_Runtime
        DESTINATION .
)]]
install(
        IMPORTED_RUNTIME_ARTIFACTS GameEngine_UI
        DESTINATION .
)
install(
        DIRECTORY data/Resources DESTINATION .
)

if (PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif ()
