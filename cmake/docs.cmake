# ---- Dependencies ----

set(extract_timestamps "")
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24")
    set(extract_timestamps DOWNLOAD_EXTRACT_TIMESTAMP YES)
endif ()

include(FetchContent)
FetchContent_Declare(
        mcss URL
        https://github.com/mosra/m.css/archive/refs/heads/master.zip
        SOURCE_DIR "${PROJECT_BINARY_DIR}/mcss"
        UPDATE_DISCONNECTED YES
        ${extract_timestamps}
)
FetchContent_MakeAvailable(mcss)

find_package(Python3 3.6 REQUIRED)

# ---- Declare documentation target ----

set(
        DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/docs"
        CACHE PATH "Path for the generated Doxygen documentation"
)

set(working_dir "${PROJECT_BINARY_DIR}/docs")

foreach (file IN ITEMS Doxyfile conf.py)
    configure_file("docs/${file}.in" "${working_dir}/${file}" @ONLY)
endforeach ()

set(mcss_script "${mcss_SOURCE_DIR}/documentation/doxygen.py")
set(postprocess_script "${mcss_SOURCE_DIR}/css/postprocess.py")
set(config "${working_dir}/conf.py")

add_custom_target(
        docs
        COMMAND "${CMAKE_COMMAND}" -E remove_directory
        "${DOXYGEN_OUTPUT_DIRECTORY}/html"
        "${DOXYGEN_OUTPUT_DIRECTORY}/xml"
        "${DOXYGEN_OUTPUT_DIRECTORY}/templates"
        "${DOXYGEN_OUTPUT_DIRECTORY}/css"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/docs/custom-pages" "${DOXYGEN_OUTPUT_DIRECTORY}/html"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/docs/templates" "${DOXYGEN_OUTPUT_DIRECTORY}/templates"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/docs/css" "${DOXYGEN_OUTPUT_DIRECTORY}/css"
        COMMAND cd css && "${Python3_EXECUTABLE}" "${postprocess_script}" "m-dark.css" --output=m-dark.compiled.css
        COMMAND cd ../ && "${Python3_EXECUTABLE}" "${mcss_script}" --templates=templates/doxygen "${config}"

        COMMENT "Building documentation using Doxygen and m.css"
        WORKING_DIRECTORY "${working_dir}"
        VERBATIM
)
