
add_library(
        MegaEngineEditor_lib OBJECT
        GameEngine.cpp
        mvp/main/MainWindow.cpp
        graphic/viewport/OpenGL.cpp
        graphic/viewport/OpenGL.h
        graphic/viewport/OpenGLFwd.h
        graphic/viewport/Camera.cpp
        graphic/viewport/Camera.h
        graphic/viewport/CameraFwd.h
        graphic/shaders/ShadersList.cpp
        graphic/shaders/ShadersList.h
        graphic/ProgramWideSettings.cpp
        graphic/objects/Door.h
        graphic/renderers/instanced/InstancedSceneMesh.cpp
        graphic/renderers/renderBuffers/RenderColorBuffer.h
        graphic/renderers/renderBuffers/RenderIndicesBuffer.h
        graphic/renderers/SceneMeshBase.cpp
        graphic/renderers/instanced/MeshInstance.cpp
        graphic/renderers/SceneMesh.cpp
        graphic/renderers/SceneMesh.h
        graphic/renderers/SceneMeshFwd.h
        graphic/UIUtils.cpp
        project/toolchain/ToolchainUtils.cpp
        project/toolchain/ToolchainUtils.h
        project/toolchain/ToolchainSettings.cpp
        project/toolchain/ToolchainSettings.h
        project/ProjectSettings.cpp
        project/ProjectSettings.h
        project/Project.cpp
        project/Project.h
        project/ProjectFwd.h
        project/CreateProjectWindow.cpp
        project/CreateProjectWindow.h
        project/CreateProjectWindowFwd.h
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
        graphic/EngineContext.h
        graphic/EngineContext.cpp
        project/sceneObjects/SceneObject.cpp
        project/sceneObjects/SceneObject.h
        ui/customWidgets/TreeWidget.cpp
        ui/customWidgets/TreeWidget.h
        ui/widgetWindows/TreeObjectWindow.cpp
        ui/widgetWindows/TreeObjectWindow.h
        project/sceneObjects/World.cpp
        project/sceneObjects/World.h
        project/sceneObjects/SimpleMesh.cpp
        project/sceneObjects/SimpleMesh.h
        ui/utils/ErrorDialog.cpp
        ui/utils/ErrorDialog.h
        ui/widgetWindows/ObjectPropertiesWindow.cpp
        ui/widgetWindows/ObjectPropertiesWindow.h
        ui/widgetWindows/CenterWindow.cpp
        ui/widgetWindows/CenterWindow.h
        ui/widgetWindows/projectExplorer/ProjectExplorerWindow.cpp
        ui/widgetWindows/projectExplorer/ProjectExplorerWindow.h
        ui/widgetWindows/projectExplorer/ProjectExplorerEntry.cpp
        ui/widgetWindows/projectExplorer/ProjectExplorerEntry.h
        mvp/main/editors/IEditorPresenter.h
        mvp/main/editors/CppEditor.cpp
        mvp/main/editors/CppEditor.h
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
        mvp/creatingProject/ProjectCreatingTask.h
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
        ui/widgetWindows/projectExplorer/FileEntry.h
        ui/widgetWindows/projectExplorer/DirectoryEntry.h
        project/generatedFiles/GeneratedFileEntry.cpp
        project/generatedFiles/GeneratedFileEntry.h
        project/generatedFiles/ScenesInfo.cpp
        project/generatedFiles/ScenesInfo.h
        project/SceneTypeRegister.cpp
        project/SceneTypeRegister.h
        project/scenes/Scene.cpp
        project/scenes/Scene.h
        project/generatedFiles/SceneInfo.cpp
        project/generatedFiles/SceneInfo.h
        ui/widgetWindows/projectExplorer/FileEntry.cpp
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
        mvp/main/editors/sceneEditor/ModelSceneEditor.cpp
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
        cmdOptions/options/SdkPathOption.cpp
        cmdOptions/options/SdkPathOption.h
        cmdOptions/options/SdkVersionOption.cpp
        cmdOptions/options/SdkVersionOption.h

)

target_precompile_headers(MegaEngineEditor_lib
        PRIVATE common.h
)

target_include_directories(
        MegaEngineEditor_lib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/editor>"
)
target_include_directories(
        MegaEngineEditor_lib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/include>"
)
target_include_directories(
        MegaEngineEditor_lib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)

target_link_libraries(MegaEngineEditor_lib PRIVATE MegaEngineUtils)

target_compile_features(MegaEngineEditor_lib PUBLIC cxx_std_23)


find_package(ZLIB REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE ZLIB::ZLIB)

# Each component imports a target:
# TARGETS: PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX
find_package(pcre2 CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX)

pkg_check_modules(LIBFFI REQUIRED IMPORTED_TARGET libffi)
target_link_libraries(MegaEngineEditor_lib PRIVATE PkgConfig::LIBFFI)

find_package(unofficial-libffi CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE unofficial::libffi::libffi)

find_package(Intl REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE Intl::Intl) # since CMake 3.20

find_package(unofficial-brotli CONFIG REQUIRED)
# Brotli decoder library
target_link_libraries(MegaEngineEditor_lib PRIVATE unofficial::brotli::brotlidec)
# Brotli encoder library
target_link_libraries(MegaEngineEditor_lib PRIVATE unofficial::brotli::brotlienc)

find_package(BZip2 REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE BZip2::BZip2)

find_package(PNG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE PNG::PNG)

find_package(Freetype REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE Freetype::Freetype) # since CMake 3.10

# this is heuristically generated, and may not be correct
find_package(unofficial-libuuid CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE unofficial::UUID::uuid)

# this is heuristically generated, and may not be correct
find_package(expat CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE expat::expat)

find_package(Fontconfig REQUIRED) # since CMake 3.14
target_link_libraries(MegaEngineEditor_lib PRIVATE Fontconfig::Fontconfig)

# cairo
pkg_check_modules(cairo REQUIRED IMPORTED_TARGET cairo)
target_link_libraries(MegaEngineEditor_lib PRIVATE PkgConfig::cairo)

# cairo-script-interpreter
pkg_check_modules(cairo-script-interpreter REQUIRED IMPORTED_TARGET cairo-script-interpreter)
target_link_libraries(MegaEngineEditor_lib PRIVATE PkgConfig::cairo-script-interpreter)

# this is heuristically generated, and may not be correct
find_package(sigc++-3 CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE sigc-3.0)

pkg_check_modules(CAIROMM REQUIRED IMPORTED_TARGET cairomm-1.16)
target_link_libraries(MegaEngineEditor_lib PRIVATE PkgConfig::CAIROMM)

find_path(EGL_REGISTRY_INCLUDE_DIRS "EGL/egl.h")
target_include_directories(MegaEngineEditor_lib PRIVATE ${EGL_REGISTRY_INCLUDE_DIRS})

find_package(fmt REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE fmt::fmt)

find_package(LibLZMA REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE LibLZMA::LibLZMA)

# this is heuristically generated, and may not be correct
find_package(harfbuzz CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE harfbuzz::harfbuzz)

# this is heuristically generated, and may not be correct
find_package(unofficial-pcre CONFIG REQUIRED)
# note: 1 additional targets are not displayed.
target_link_libraries(MegaEngineEditor_lib PRIVATE unofficial::pcre::pcre unofficial::pcre::pcre16 unofficial::pcre::pcre32 unofficial::pcre::pcrecpp)

find_package(JPEG REQUIRED)
target_include_directories(MegaEngineEditor_lib PRIVATE JPEG::JPEG)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE glfw)

pkg_check_modules(GTKMM gtkmm-4.0)
include_directories(${GTKMM_INCLUDE_DIRS})
target_link_libraries(MegaEngineEditor_lib PRIVATE ${GTKMM_LINK_LIBRARIES})

# this is heuristically generated, and may not be correct
find_package(SQLiteCpp CONFIG REQUIRED)
target_link_libraries(MegaEngineEditor_lib PRIVATE SQLiteCpp)
