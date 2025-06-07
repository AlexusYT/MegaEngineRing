set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}/data")
execute_process(
        COMMAND ${CMAKE_BINARY_DIR}/ImGuiB2C "${RESOURCE_PATH}/fonts/NotoSans/static/NotoSans-Regular.ttf" font_noto_sans_regular
        RESULT_VARIABLE RESULT_B2C_EXECUTION
        ERROR_VARIABLE RESULT_B2C_EXECUTION_ERROR
        OUTPUT_VARIABLE RESULT_B2C_EXECUTION_OUTPUT
)
if (NOT ${RESULT_B2C_COMPILATION} EQUAL 0)
    message(FATAL_ERROR "Failed to execute ImGuiB2C utility:
    ${RESULT_B2C_EXECUTION}
    The utility is used to compress the TTF files and save it as C-array. It is required to run, so, try to fix the error
    ")
endif ()

configure_file(CompressedFonts.h.in CompressedFonts.h)

set(EDITOR_SOURCES
        GameEngine.cpp
        project/Project.cpp
        project/Project.h
        Globals.cpp
        Globals.h
        mvp/treeObjectWindow/ViewObjectsTree.cpp
        mvp/treeObjectWindow/ViewObjectsTree.h
        mvp/objectProperties/ViewObjectProperties.cpp
        mvp/objectProperties/ViewObjectProperties.h
        mvp/projectExplorer/ViewProjectExplorer.cpp
        mvp/projectExplorer/ViewProjectExplorer.h
        project/generators/cpp/CppGenerator.cpp
        project/generators/cpp/CppGenerator.h
        project/generators/cpp/CppClass.cpp
        project/generators/cpp/CppClass.h
        project/generators/cpp/CppMethod.cpp
        project/generators/cpp/CppMethod.h
        project/generators/GeneratorElement.cpp
        project/generators/GeneratorElement.h
        project/generators/cpp/AccessModifier.h
        project/generators/cpp/CppBodyStatement.h
        project/generators/cpp/CppCustomStatement.cpp
        project/generators/cpp/CppCustomStatement.h
        mvp/IPresenter.h
        mvp/IView.h
        mvp/sceneEditor/ViewSceneEditor.cpp
        mvp/sceneEditor/ViewSceneEditor.h
        project/generators/cpp/CppHeaderFile.cpp
        project/generators/cpp/CppHeaderFile.h
        project/generators/cpp/CppFile.cpp
        project/generators/cpp/CppFile.h
        project/generators/cpp/CppDefinition.h
        project/generators/cpp/CppDeclaration.h
        project/generators/cpp/CppClassMember.cpp
        project/generators/cpp/CppClassMember.h
        project/generators/cpp/CppExternC.cpp
        project/generators/cpp/CppExternC.h
        mvp/projectExplorer/IViewProjectExplorer.h
        mvp/projectExplorer/IModelProjectExplorer.h
        mvp/projectExplorer/ModelProjectExplorer.cpp
        mvp/projectExplorer/ModelProjectExplorer.h
        mvp/projectExplorer/IPresenterProjectExplorer.h
        mvp/projectExplorer/PresenterProjectExplorer.cpp
        mvp/projectExplorer/PresenterProjectExplorer.h
        mvp/sceneEditor/ResourcesContext.cpp
        mvp/sceneEditor/ResourcesContext.h
        mvp/projectExplorer/entries/ProjectExplorerElement.cpp
        mvp/projectExplorer/entries/ProjectExplorerElement.h
        project/LoadedScene.cpp
        project/LoadedScene.h
        mvp/treeObjectWindow/ModelObjectsTree.cpp
        mvp/treeObjectWindow/ModelObjectsTree.h
        mvp/treeObjectWindow/PresenterObjectsTree.cpp
        mvp/treeObjectWindow/PresenterObjectsTree.h
        mvp/objectProperties/ModelObjectProperties.cpp
        mvp/objectProperties/ModelObjectProperties.h
        mvp/objectProperties/PresenterObjectProperties.cpp
        mvp/objectProperties/PresenterObjectProperties.h
        mvp/sceneEditor/SceneOverlayElements.cpp
        mvp/sceneEditor/SceneOverlayElements.h
        graphic/shaders/SimpleShader.cpp
        graphic/shaders/SimpleShader.h
        mvp/contexts/UiWindowContext.cpp
        mvp/contexts/UiWindowContext.h
        mvp/dialogs/confirmationDialog/ConfirmationDialog.cpp
        mvp/dialogs/confirmationDialog/ConfirmationDialog.h
        mvp/prefabEditor/ViewPrefabEditor.cpp
        mvp/prefabEditor/ViewPrefabEditor.h
        mvp/prefabEditor/IModelPrefabEditor.h
        mvp/prefabEditor/ModelPrefabEditor.cpp
        mvp/prefabEditor/ModelPrefabEditor.h
        mvp/prefabEditor/IPresenterPrefabEditor.h
        mvp/prefabEditor/PresenterPrefabEditor.cpp
        mvp/prefabEditor/PresenterPrefabEditor.h
        mvp/onlineImport/ViewOnlineImport.cpp
        mvp/onlineImport/ViewOnlineImport.h
        mvp/onlineImport/ModelOnlineImport.cpp
        mvp/onlineImport/ModelOnlineImport.h
        mvp/onlineImport/PresenterOnlineImport.cpp
        mvp/onlineImport/PresenterOnlineImport.h
        mvp/onlineImport/sketchfab/SketchfabAccount.cpp
        mvp/onlineImport/sketchfab/SketchfabAccount.h
        mvp/onlineImport/sketchfab/SketchfabCache.cpp
        mvp/onlineImport/sketchfab/SketchfabCache.h
        mvp/onlineImport/sketchfab/SketchfabSearch.cpp
        mvp/onlineImport/sketchfab/SketchfabSearch.h
        mvp/scenePreview/IViewScenePreview.h
        mvp/scenePreview/ViewScenePreview.cpp
        mvp/scenePreview/ViewScenePreview.h
        mvp/scenePreview/IPresenterScenePreview.h
        mvp/scenePreview/IModelScenePreview.h
        mvp/scenePreview/prefab/PresenterPrefabPreview.cpp
        mvp/scenePreview/prefab/PresenterPrefabPreview.h
        mvp/scenePreview/prefab/ModelPrefabPreview.cpp
        mvp/scenePreview/prefab/ModelPrefabPreview.h
        mvp/scenePreview/ModelScenePreview.cpp
        mvp/scenePreview/ModelScenePreview.h
        mvp/scenePreview/onlineImport/PresenterOnlineImportPreview.cpp
        mvp/scenePreview/onlineImport/PresenterOnlineImportPreview.h
        mvp/scenePreview/sceneEditor/PresenterSceneEditorPreview.cpp
        mvp/scenePreview/sceneEditor/PresenterSceneEditorPreview.h
        mvp/scenePreview/PresenterScenePreview.h
        mvp/scenePreview/PresenterScenePreview.cpp
        mvp/editor/Editor.cpp
        mvp/editor/Editor.h
        mvp/editor/EditorUi.cpp
        mvp/editor/EditorUi.h
        mvp/sceneEditor/NodeSelectionHelper.h
        Utils.cpp
        Utils.h
        mvp/editor/settings/ViewSettingsWindow.cpp
        mvp/editor/settings/ViewSettingsWindow.h
        mvp/editor/settings/ModelSettingsWindow.cpp
        mvp/editor/settings/ModelSettingsWindow.h
        mvp/editor/settings/PresenterSettingsWindow.cpp
        mvp/editor/settings/PresenterSettingsWindow.h
        mvp/editor/settings/categories/OtherCategory.cpp
        mvp/editor/settings/categories/OtherCategory.h
        mvp/editor/settings/Settings.cpp
        mvp/editor/settings/Settings.h
        mvp/editor/settings/categories/SettingsCategory.h
        mvp/editor/settings/categories/GeneralCategory.cpp
        mvp/editor/settings/categories/GeneralCategory.h
        I18N.cpp
        I18N.h

)

add_library(MegaEngineEditor_lib OBJECT CompressedFonts.h ${EDITOR_SOURCES})

configure_gettext(
        DOMAIN "mer"
        TARGET_NAME "MegaEngineEditor_lib-gettext"
        SOURCES ${EDITOR_SOURCES}
        POTFILE_DESTINATION "${PROJECT_SOURCE_DIR}/data/locale"
        GMOFILE_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/locale"
        BUILD_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/locale"
        INSTALL_DESTINATION "${CMAKE_INSTALL_FULL_LOCALEDIR}"
        XGETTEXT_ARGS
        "--keyword=_" "--keyword=N_" "--keyword=trp:1,2" "--keyword=tr" "--keyword=trs" "--keyword=get" "--add-comments=translators:"
        "--package-name=${PROJECT_NAME}" "--package-version=${PROJECT_VERSION}" "--foreign-user"
        "--copyright-holder=\"Timofeev (Alexus_XX) Alexander\"" "--from-code=UTF-8"
        MSGINIT_ARGS "--no-translator"
        LANGUAGES "en_US" "ru_RU")
target_compile_definitions(MegaEngineEditor_lib PUBLIC LOCALE_DIR="${CMAKE_INSTALL_FULL_LOCALEDIR}")
add_dependencies(MegaEngineEditor_lib MegaEngineEditor_lib-gettext)
#[[find_program(XGETTEXT_CMD xgettext REQUIRED)
SET(_potFile ${PROJECT_NAME}.pot)
ADD_CUSTOM_COMMAND(OUTPUT ${_potFile}
        COMMAND ${XGETTEXT_CMD} ${_xgettext_option_list} -o ${_potFile} ${EDITOR_SOURCES}
        DEPENDS ${EDITOR_SOURCES}
        WORKING_DIRECTORY "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/editor>"
        COMMENT "Extract translatable messages to ${_potFile}"
)
gettext_process_pot_file(${_potFile} LANGUAGES data/locale/ru_RU/ui)
ADD_CUSTOM_TARGET(pot_file ${_all}
        DEPENDS ${_potFile}
)]]
target_compile_options(MegaEngineEditor_lib PRIVATE -Werror)

target_precompile_headers(MegaEngineEditor_lib
        PRIVATE common.h
)

target_include_directories(
        MegaEngineEditor_lib ${warning_guard}
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
)
target_include_directories(
        MegaEngineEditor_lib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/editor>"
)

target_link_libraries(MegaEngineEditor_lib PRIVATE MegaEngineSDK_shared)
target_compile_definitions(MegaEngineEditor_lib PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_compile_features(MegaEngineEditor_lib PRIVATE cxx_std_23)

target_link_libraries(MegaEngineEditor_lib PRIVATE ImGUI)
target_link_libraries(MegaEngineEditor_lib PRIVATE ImGuiFileDialog)

#find_package(nlohmann_json CONFIG REQUIRED)
pkg_check_modules(nlohmann_json REQUIRED IMPORTED_TARGET nlohmann_json)
target_link_libraries(MegaEngineEditor_lib PRIVATE ${nlohmann_json_LIBRARIES})
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

target_link_libraries(MegaEngineEditor_lib PRIVATE curl)