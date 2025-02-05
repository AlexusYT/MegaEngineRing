
list(JOIN CMAKE_PROGRAM_PATH ":" PROGRAM_PATH)

message(STATUS "${PROGRAM_PATH}")
set(RESOURCE_PATH "${CMAKE_SOURCE_DIR}/data")
if (MINGW)
    set(EXPORT_COMMAND set PATH="${PROGRAM_PATH};$ENV{PATH}")
else ()
    set(EXPORT_COMMAND export PATH="${PROGRAM_PATH};$ENV{PATH}")
endif ()
add_custom_command(
        OUTPUT resources.cpp
        COMMAND PATH="${PROGRAM_PATH}:$ENV{PATH}" glib-compile-resources --target=resources.cpp --generate-source --internal
        --sourcedir="${RESOURCE_PATH}/css"
        --sourcedir="${RESOURCE_PATH}/icons/32x32/actions"
        --sourcedir="${RESOURCE_PATH}/ui"
        --sourcedir="${RESOURCE_PATH}/paned-layouts"
        --sourcedir="${RESOURCE_PATH}/_builtin_/shaders"
        "${RESOURCE_PATH}/resources.xml"
        COMMAND dir ".."
        DEPENDS "${RESOURCE_PATH}/resources.xml" "${RESOURCE_PATH}"
)

add_library(
        MegaEngineEditor_lib OBJECT
        resources.cpp
        GameEngine.cpp
        mvp/main/MainWindow.cpp
        project/Project.cpp
        project/Project.h
        mvp/startup/StartupWindow.cpp
        mvp/startup/StartupWindow.h
        cmdOptions/EngineOptionGroup.cpp
        cmdOptions/EngineOptionGroup.h
        cmdOptions/options/LogPathOption.cpp
        cmdOptions/options/LogPathOption.h
        cmdOptions/options/Option.cpp
        cmdOptions/options/Option.h
        Globals.cpp
        Globals.h
        project/sceneObjects/EditorSceneObject.cpp
        project/sceneObjects/EditorSceneObject.h
        ui/customWidgets/TreeWidget.cpp
        ui/customWidgets/TreeWidget.h
        mvp/main/treeObjectWindow/ViewObjectsTree.cpp
        mvp/main/treeObjectWindow/ViewObjectsTree.h
        ui/utils/ErrorDialog.cpp
        ui/utils/ErrorDialog.h
        mvp/main/objectProperties/ViewObjectProperties.cpp
        mvp/main/objectProperties/ViewObjectProperties.h
        mvp/main/centerWindow/ViewCenterWindow.cpp
        mvp/main/centerWindow/ViewCenterWindow.h
        mvp/main/projectExplorer/ViewProjectExplorer.cpp
        mvp/main/projectExplorer/ViewProjectExplorer.h
        ui/widgetWindows/projectExplorer/ProjectExplorerEntry.cpp
        ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h
        mvp/main/editors/IPresenterSceneEditor.h
        project/generatedFiles/ApplicationInfo.cpp
        project/generatedFiles/ApplicationInfo.h
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
        mvp/creatingProject/ProjectCreatingWindow.cpp
        mvp/creatingProject/ProjectCreatingWindow.h
        ui/widgetWindows/LogView.cpp
        ui/widgetWindows/LogView.h
        mvp/creatingProject/ModelCreatingProject.h
        mvp/creatingProject/IViewCreatingProject.h
        mvp/creatingProject/PresenterCreatingProject.cpp
        mvp/creatingProject/PresenterCreatingProject.h
        mvp/creatingProject/IModelCreatingProject.h
        mvp/IPresenter.h
        mvp/IView.h
        mvp/startup/IViewStartup.h
        mvp/startup/IModelStartup.h
        mvp/startup/ModelStartup.cpp
        mvp/startup/ModelStartup.h
        mvp/startup/PresenterStartup.cpp
        mvp/startup/PresenterStartup.h
        project/generatedFiles/GeneratedFiles.cpp
        project/generatedFiles/GeneratedFiles.h
        mvp/main/IViewMain.h
        mvp/main/IModelMain.h
        mvp/main/ModelMain.cpp
        mvp/main/ModelMain.h
        mvp/main/PresenterMain.cpp
        mvp/main/PresenterMain.h
        mvp/ThreadDispatcher.cpp
        mvp/ThreadDispatcher.h
        project/generatedFiles/GeneratedFileEntry.cpp
        project/generatedFiles/GeneratedFileEntry.h
        mvp/main/editors/appInfo/AppInfoEditor.cpp
        mvp/main/editors/appInfo/AppInfoEditor.h
        mvp/main/editors/appInfo/IViewAppInfo.h
        mvp/main/editors/appInfo/IModelAppInfo.h
        mvp/main/editors/appInfo/ModelAppInfo.cpp
        mvp/main/editors/appInfo/ModelAppInfo.h
        mvp/main/editors/appInfo/PresenterAppInfo.cpp
        mvp/main/editors/appInfo/PresenterAppInfo.h
        mvp/main/editors/sceneEditor/IViewSceneEditor.h
        mvp/main/editors/sceneEditor/IModelSceneEditor.h
        mvp/main/editors/sceneEditor/PresenterSceneEditor.cpp
        mvp/main/editors/sceneEditor/PresenterSceneEditor.h
        mvp/main/editors/sceneEditor/ModelSceneEditor.h
        mvp/main/editors/sceneEditor/ViewSceneEditor.cpp
        mvp/main/editors/sceneEditor/ViewSceneEditor.h
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
        mvp/main/objectProperties/ObjectPropertyEntry.cpp
        mvp/main/objectProperties/ObjectPropertyEntry.h
        project/scripting/ScriptParser.cpp
        project/scripting/ScriptParser.h
        project/scripting/ScriptNode.cpp
        project/scripting/ScriptNode.h
        project/generatedFiles/GraphicsScript.cpp
        project/generatedFiles/GraphicsScript.h
        mvp/main/editors/graphicsScriptEditor/IModelGraphicsScriptEditor.h
        mvp/main/editors/graphicsScriptEditor/ModelGraphicsScriptEditor.cpp
        mvp/main/editors/graphicsScriptEditor/ModelGraphicsScriptEditor.h
        mvp/main/editors/graphicsScriptEditor/IViewGraphicsScriptEditor.h
        mvp/main/editors/graphicsScriptEditor/ViewGraphicsScriptEditor.cpp
        mvp/main/editors/graphicsScriptEditor/ViewGraphicsScriptEditor.h
        mvp/main/editors/graphicsScriptEditor/PresenterGraphicsScriptEditor.cpp
        mvp/main/editors/graphicsScriptEditor/PresenterGraphicsScriptEditor.h
        mvp/main/editors/graphicsScriptEditor/canvas/CanvasWidget.cpp
        mvp/main/editors/graphicsScriptEditor/canvas/CanvasWidget.h
        mvp/main/editors/graphicsScriptEditor/canvas/CanvasElement.cpp
        mvp/main/editors/graphicsScriptEditor/canvas/CanvasElement.h
        project/scripting/ExtensionScriptNode.cpp
        project/scripting/ExtensionScriptNode.h
        project/scripting/FakeScriptNode.cpp
        project/scripting/FakeScriptNode.h
        mvp/ApplicationController.cpp
        mvp/ApplicationController.h
        mvp/IApplicationController.h
        mvp/contexts/IWidgetContext.h
        mvp/contexts/ApplicationContext.cpp
        mvp/contexts/ApplicationContext.h
        mvp/PresenterBase.cpp
        mvp/PresenterBase.h
        mvp/main/projectExplorer/IViewProjectExplorer.h
        mvp/main/projectExplorer/IModelProjectExplorer.h
        mvp/main/projectExplorer/ModelProjectExplorer.cpp
        mvp/main/projectExplorer/ModelProjectExplorer.h
        mvp/main/projectExplorer/IPresenterProjectExplorer.h
        mvp/main/projectExplorer/PresenterProjectExplorer.cpp
        mvp/main/projectExplorer/PresenterProjectExplorer.h
        mvp/main/centerWindow/IViewCenterWindow.h
        mvp/main/centerWindow/IModelCenterWindow.h
        mvp/main/centerWindow/ModelCenterWindow.cpp
        mvp/main/centerWindow/ModelCenterWindow.h
        mvp/main/centerWindow/IPresenterCenterWindow.h
        mvp/main/centerWindow/PresenterCenterWindow.cpp
        mvp/main/centerWindow/PresenterCenterWindow.h
        mvp/main/centerWindow/ITab.h
        mvp/main/centerWindow/TabPlaceholder.cpp
        mvp/main/centerWindow/TabPlaceholder.h
        mvp/main/centerWindow/ITabPlaceholder.h
        mvp/main/editors/sceneEditor/ModelSceneEditor.cpp
        mvp/main/editors/sceneEditor/explorerObjects/ExplorerObject.cpp
        mvp/main/editors/sceneEditor/explorerObjects/ExplorerObject.h
        mvp/main/editors/sceneEditor/explorerObjects/RootExplorerObject.h
        mvp/main/editors/sceneEditor/explorerObjects/SceneExplorerObject.cpp
        mvp/main/editors/sceneEditor/explorerObjects/SceneExplorerObject.h
        mvp/main/editors/sceneEditor/ResourcesContext.cpp
        mvp/main/editors/sceneEditor/ResourcesContext.h
        ui/customWidgets/CustomTreeView.cpp
        ui/customWidgets/CustomTreeView.h
        ui/customWidgets/TreeElementBase.h
        ui/customWidgets/CustomSignalListItemFactory.h
        ui/customWidgets/CustomSignalListItemFactory.cpp
        ui/customWidgets/multipaned/MultiPaned.cpp
        ui/customWidgets/multipaned/MultiPaned.h
        mvp/contexts/MultiPanedContext.cpp
        mvp/contexts/MultiPanedContext.h
        ui/customWidgets/multipaned/MultiPanedPanel.cpp
        ui/customWidgets/multipaned/MultiPanedPanel.h
        ui/customWidgets/multipaned/MultiPanedSide.h
        mvp/main/projectExplorer/entries/ProjectExplorerElement.cpp
        mvp/main/projectExplorer/entries/ProjectExplorerElement.h
        mvp/main/IPresenterMain.h
        project/LoadedScene.cpp
        project/LoadedScene.h
        mvp/main/editors/sceneEditor/EditorCameraScript.cpp
        mvp/main/editors/sceneEditor/EditorCameraScript.h
        mvp/main/treeObjectWindow/IViewObjectsTree.h
        mvp/main/treeObjectWindow/IModelObjectsTree.h
        mvp/main/treeObjectWindow/ModelObjectsTree.h
        mvp/main/treeObjectWindow/IPresenterObjectsTree.h
        mvp/main/treeObjectWindow/PresenterObjectsTree.cpp
        mvp/main/treeObjectWindow/PresenterObjectsTree.h
        mvp/main/objectProperties/IViewObjectProperties.h
        mvp/main/objectProperties/IModelObjectProperties.h
        mvp/main/objectProperties/ModelObjectProperties.h
        mvp/main/objectProperties/IPresenterObjectProperties.h
        mvp/main/objectProperties/PresenterObjectProperties.cpp
        mvp/main/objectProperties/PresenterObjectProperties.h
        mvp/resourceEditor/IModelResourceEditor.h
        mvp/resourceEditor/ModelResourceEditor.cpp
        mvp/resourceEditor/ModelResourceEditor.h
        mvp/resourceEditor/IViewResourceEditor.h
        mvp/resourceEditor/ViewResourceEditor.cpp
        mvp/resourceEditor/ViewResourceEditor.h
        mvp/resourceEditor/IPresenterResourceEditor.h
        mvp/resourceEditor/PresenterResourceEditor.cpp
        mvp/resourceEditor/PresenterResourceEditor.h
        mvp/resourceEditor/readers/FileResourceReader.h
        mvp/resourceEditor/readers/ObjFileResourceReader.cpp
        mvp/resourceEditor/readers/ObjFileResourceReader.h
        mvp/resourceEditor/savers/Model3DResourceSaver.cpp
        mvp/resourceEditor/savers/Model3DResourceSaver.h
        mvp/resourceSelection/IModelResourceSelection.h
        mvp/resourceSelection/ModelResourceSelection.cpp
        mvp/resourceSelection/ModelResourceSelection.h
        mvp/resourceSelection/IViewResourceSelection.h
        mvp/resourceSelection/ViewResourceSelection.h
        mvp/resourceSelection/ViewResourceSelection.cpp
        mvp/resourceSelection/IPresenterResourceSelection.h
        mvp/resourceSelection/PresenterResourceSelection.h
        mvp/resourceSelection/PresenterResourceSelection.cpp
        mvp/contexts/ViewportContext.cpp
        mvp/contexts/ViewportContext.h
        mvp/main/PanedLayoutTab.cpp
        mvp/main/PanedLayoutTab.h
        mvp/resourceEditor/readers/PngFileResourceReader.cpp
        mvp/resourceEditor/readers/PngFileResourceReader.h
        mvp/resourceEditor/EditingResourceList.cpp
        mvp/resourceEditor/EditingResourceList.h
        ui/utils/UiUtils.cpp
        ui/utils/UiUtils.h
        mvp/resourceEditor/savers/TextureResourceSaver.cpp
        mvp/resourceEditor/savers/TextureResourceSaver.h
        mvp/resourceEditor/savers/MaterialResourceSaver.cpp
        mvp/resourceEditor/savers/MaterialResourceSaver.h
        StringInputStream.cpp
        StringInputStream.h
        ui/customWidgets/resourceSelector/ResourceSelectorWidget.cpp
        ui/customWidgets/resourceSelector/ResourceSelectorWidget.h
        ui/customWidgets/resourceSelector/ISourceSelection.h
        ui/customWidgets/resourceSelector/SourceSelectionNone.cpp
        ui/customWidgets/resourceSelector/SourceSelectionNone.h
        ui/customWidgets/resourceSelector/resources/SourceSelectionModel3D.cpp
        ui/customWidgets/resourceSelector/resources/SourceSelectionModel3D.h
        mvp/main/objectProperties/PropertyRenderer.cpp
        mvp/main/objectProperties/PropertyRenderer.h
        mvp/main/objectProperties/PropertyRendererType.h
        project/EditorFileSystemResourceBundle.cpp
        project/EditorFileSystemResourceBundle.h
        ui/customWidgets/resourceSelector/ISourceSelectionResult.h
        ui/customWidgets/resourceSelector/SourceSelection.cpp
        ui/customWidgets/resourceSelector/SourceSelection.h
        ui/customWidgets/resourceSelector/resources/SourceSelectionMaterial.cpp
        ui/customWidgets/resourceSelector/resources/SourceSelectionMaterial.h
        ui/customWidgets/resourceSelector/resources/SourceSelectionResource.cpp
        ui/customWidgets/resourceSelector/resources/SourceSelectionResource.h
        ui/customWidgets/resourceSelector/resources/TypedSourceSelectionResult.h
        ui/customWidgets/resourceSelector/resources/SourceSelectionTexture.cpp
        ui/customWidgets/resourceSelector/resources/SourceSelectionTexture.h
        ui/customWidgets/resourceSelector/SourceSelectionColor.cpp
        ui/customWidgets/resourceSelector/SourceSelectionColor.h
        mvp/resourceEditor/savers/ResourceSaverBase.cpp
        mvp/resourceEditor/savers/ResourceSaverBase.h
        mvp/main/editors/sceneEditor/SceneOverlayElements.cpp
        mvp/main/editors/sceneEditor/SceneOverlayElements.h
        graphic/shaders/SimpleShader.cpp
        graphic/shaders/SimpleShader.h

)

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

target_compile_features(MegaEngineEditor_lib PRIVATE cxx_std_23)


pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(MegaEngineEditor_lib PRIVATE ${GTKMM_LINK_LIBRARIES})

# this is heuristically generated, and may not be correct
find_package(SQLiteCpp CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE SQLiteCpp)

#find_package(nlohmann_json CONFIG REQUIRED)
pkg_check_modules(nlohmann_json REQUIRED IMPORTED_TARGET nlohmann_json)
target_link_libraries(MegaEngineEditor_lib PRIVATE ${nlohmann_json_LIBRARIES})
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)