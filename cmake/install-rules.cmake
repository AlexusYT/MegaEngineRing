install(
        IMPORTED_RUNTIME_ARTIFACTS MegaEngineEditor
        DESTINATION .
)
install(
        DIRECTORY data/Resources DESTINATION .
)

if (PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif ()
