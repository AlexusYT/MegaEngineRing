

set(ENGINE_SOURCES sources/main/Application.cpp
        sources/main/DefaultApplicationSettings.cpp
        sources/main/context/Window.cpp
        sources/main/context/MainWindow.cpp
        sources/main/resources/LoadedResources.cpp
        sources/main/resources/shaders/Shader.cpp
        sources/main/context/ResourcesWindow.cpp
        sources/main/resources/ResourceLoader.cpp
        sources/main/resources/Resources.cpp
        sources/main/scene/Scene.cpp
        sources/main/resources/shaders/ShaderProgram.cpp
        sources/main/resources/shaders/ShaderProgramLoader.cpp
        sources/main/buffers/ProgramWideShaderBuffer.cpp
        sources/renderer/GL.cpp
        sources/renderer/buffers/SSBO.cpp
        sources/main/scene/objects/SceneObject.cpp
        sources/main/extensions/Extension.cpp
        sources/main/extensions/BasicRenderExtension.cpp
        sources/main/extensions/cameras/CameraExtension.cpp
        sources/main/extensions/cameras/CameraKeyboardExtension.cpp
        sources/main/extensions/ExtensionRegistry.cpp
        sources/main/extensions/cameras/PerspectiveProjectionCameraMod.cpp
        sources/main/extensions/OrbitCameraExtension.cpp
        sources/main/resources/models/Model3DLoader.cpp
        sources/main/extensions/ModelRenderExtension.cpp
        sources/main/scripting/Script.cpp
        sources/main/extensions/MainObjectExtension.cpp
        sources/utils/PropertyJsonSerializers.cpp
        sources/main/resources/models/Model3DResource.cpp
        sources/main/resources/models/Model3DObject.cpp
        sources/main/render/RenderInstance.cpp
        sources/main/resources/FileSystemResourceBundle.cpp
        sources/main/resources/ResourceLoaders.cpp
        sources/main/resources/Resource.cpp
        sources/main/resources/textures/TextureResource.cpp
        sources/main/resources/textures/TextureLoader.cpp
        sources/main/extensions/LightExtension.cpp
        sources/main/light/LightInstance.cpp
        sources/main/light/LightSources.cpp
        sources/main/resources/materials/MaterialResource.cpp
        sources/main/resources/materials/MaterialLoader.cpp
        sources/main/resources/ResourceLoadResult.cpp
        sources/main/resources/materials/ColorComponent.cpp
        sources/main/prefabs/Prefab.cpp
        sources/main/prefabs/PrefabInstance.cpp
        sources/main/prefabs/elements/PrefabElementInstance.cpp
        sources/main/prefabs/elements/PrefabElementsSsbo.cpp
        sources/main/prefabs/elements/PrefabElement.cpp
        sources/main/prefabs/elements/MeshPrefabElement.cpp
        sources/main/resources/shaders/builtin/PrefabProgram.cpp
        sources/utils/Transformation.cpp
        sources/utils/Transformable.cpp
        sources/main/prefabs/PrefabSsbo.cpp
)
set(ENGINE_HEADERS include/EngineSDK/main/Application.h
        include/EngineSDK/main/context/Window.h
        include/EngineSDK/main/context/MainWindow.h
        include/EngineSDK/main/resources/LoadedResources.h
        include/EngineSDK/main/context/ResourcesWindow.h
        include/EngineSDK/main/context/IContext.h
        include/EngineSDK/main/IWindow.h
        include/EngineSDK/main/IApplicationSettings.h
        include/EngineSDK/main/DefaultApplicationSettings.h
        include/EngineSDK/main/scene/IScene.h
        include/EngineSDK/main/resources/shaders/ShaderTypeEnum.h
        include/EngineSDK/main/resources/ResourceLoader.h
        include/EngineSDK/main/resources/IResource.h
        include/EngineSDK/main/resources/Resources.h
        include/EngineSDK/main/scene/Scene.h
        include/EngineSDK/main/resources/shaders/ShaderProgram.h
        include/EngineSDK/renderer/buffers/ISSBO.h
        include/EngineSDK/renderer/buffers/SSBO.h
        include/EngineSDK/renderer/buffers/BufferUsageEnum.h
        include/EngineSDK/main/resources/shaders/ShaderProgramLoader.h
        include/EngineSDK/main/buffers/ProgramWideShaderBuffer.h
        include/EngineSDK/main/resources/shaders/Shader.h
        include/EngineSDK/main/scene/objects/ISceneObject.h
        include/EngineSDK/renderer/GL.h
        include/EngineSDK/main/extensions/cameras/ICamera.h
        include/EngineSDK/utils/ModifierKeys.h
        include/EngineSDK/utils/KeyboardKey.h
        include/EngineSDK/main/scene/objects/SceneObject.h
        include/EngineSDK/main/extensions/Extension.h
        include/EngineSDK/main/extensions/BasicRenderExtension.h
        include/EngineSDK/main/resources/IResourceLoadExecutor.h
        include/EngineSDK/main/extensions/cameras/CameraExtension.h
        include/EngineSDK/main/extensions/cameras/CameraKeyboardExtension.h
        include/EngineSDK/main/extensions/cameras/CameraMouseExtension.h
        include/EngineSDK/main/extensions/ExtensionRegistry.h
        include/EngineSDK/main/extensions/cameras/PerspectiveProjectionCameraMod.h
        include/EngineSDK/main/extensions/cameras/IProjectionCameraMod.h
        include/EngineSDK/main/extensions/cameras/OrbitCameraExtension.h
        include/EngineSDK/utils/MouseButton.h
        include/EngineSDK/main/extensions/MouseButtonExtension.h
        include/EngineSDK/main/resources/models/Model3DLoader.h
        include/EngineSDK/main/extensions/ModelRenderExtension.h
        include/EngineSDK/main/IApplication.h
        include/EngineSDK/main/scripting/IScript.h
        include/EngineSDK/main/scripting/Script.h
        include/EngineSDK/main/extensions/ExtensionProperty.h
        include/EngineSDK/main/extensions/MainObjectExtension.h
        include/EngineSDK/utils/PropertyJsonSerializers.h
        include/EngineSDK/utils/PropertyJsonSerializer.h
        include/EngineSDK/main/resources/models/Model3DResource.h
        include/EngineSDK/main/render/IRenderInstance.h
        include/EngineSDK/main/render/RenderInstanceData.h
        include/EngineSDK/main/resources/models/Model3DObject.h
        include/EngineSDK/main/resources/models/IModel3DObject.h
        include/EngineSDK/main/resources/models/IModel3DResource.h
        include/EngineSDK/main/render/RenderInstance.h
        include/EngineSDK/main/render/IInstancedRender.h
        include/EngineSDK/main/resources/IResourceBundle.h
        include/EngineSDK/main/resources/FileSystemResourceBundle.h
        include/EngineSDK/main/resources/ResourceLoaders.h
        include/EngineSDK/main/resources/IResourceLoaders.h
        include/EngineSDK/main/render/IRenderable.h
        include/EngineSDK/main/resources/Resource.h
        include/EngineSDK/main/resources/textures/ITextureResource.h
        include/EngineSDK/main/resources/textures/TextureResource.h
        include/EngineSDK/main/resources/textures/TextureMinFilter.h
        include/EngineSDK/main/resources/textures/TextureMagFilter.h
        include/EngineSDK/main/resources/textures/TextureBaseInternalFormat.h
        include/EngineSDK/main/resources/textures/Texture2DImageFormat.h
        include/EngineSDK/main/resources/textures/Texture2DType.h
        include/EngineSDK/main/resources/ResourceType.h
        include/EngineSDK/main/resources/textures/TextureLoader.h
        include/EngineSDK/main/extensions/LightExtension.h
        include/EngineSDK/main/light/ILightInstance.h
        include/EngineSDK/main/light/ILightSources.h
        include/EngineSDK/main/light/LightInstance.h
        include/EngineSDK/main/light/LightInstanceData.h
        include/EngineSDK/main/light/LightSources.h
        include/EngineSDK/main/resources/materials/IMaterialResource.h
        include/EngineSDK/main/resources/materials/MaterialResource.h
        include/EngineSDK/main/resources/materials/MaterialData.h
        include/EngineSDK/main/resources/materials/MaterialLoader.h
        include/EngineSDK/main/resources/ResourceLoadResult.h
        include/EngineSDK/utils/ISerializable.h
        include/EngineSDK/main/render/IInitializable.h
        include/EngineSDK/main/render/Initializable.h
        include/EngineSDK/main/resources/materials/IMaterialComponent.h
        include/EngineSDK/main/resources/materials/ColorComponent.h
        include/EngineSDK/main/prefabs/Prefab.h
        include/EngineSDK/main/prefabs/PrefabInstance.h
        include/EngineSDK/main/prefabs/elements/PrefabElementInstance.h
        include/EngineSDK/main/prefabs/elements/PrefabElementsSsbo.h
        include/EngineSDK/main/prefabs/elements/PrefabElement.h
        include/EngineSDK/main/prefabs/elements/MeshPrefabElement.h
        include/EngineSDK/main/prefabs/IPrefabElementInstance.h
        include/EngineSDK/main/prefabs/IPrefabInstance.h
        include/EngineSDK/main/resources/shaders/builtin/PrefabProgram.h
        include/EngineSDK/utils/Transformation.h
        include/EngineSDK/utils/Transformable.h
        include/EngineSDK/main/prefabs/PrefabSsbo.h
)

foreach (SOURCE IN ITEMS ${ENGINE_SOURCES})
    if (NOT ${SOURCE} MATCHES "^sources(.*)$" AND ${SOURCE} MATCHES "^(.*)\.cpp$")
        message(FATAL_ERROR "
        Fatal Error when checking SDK Sources.
        File ${SOURCE} is a source file, but not in the sources directory
        Move the file to the sources directory")
    endif ()
    if (${SOURCE} MATCHES "^(.*)\.h$")
        message(FATAL_ERROR "
        Fatal Error when checking SDK Sources.
        File ${SOURCE} is a header file, but not in the ENGINE_HEADERS variable
        Move the file to the ENGINE_HEADERS variable in CMakeLists.txt")
    endif ()
endforeach ()

foreach (HEADER IN ITEMS ${ENGINE_HEADERS})
    if (NOT ${HEADER} MATCHES "^include(.*)$" AND ${HEADER} MATCHES "^(.*)\.h$")
        message(FATAL_ERROR "
        Fatal Error when checking SDK Sources.
        File ${HEADER} is a header file, but not in the include directory
        Move the file to the include directory")
    endif ()
    if (${HEADER} MATCHES "^(.*)\.cpp$")
        message(FATAL_ERROR "
        Fatal Error when checking SDK Sources.
        File ${HEADER} is a source file, but not in the ENGINE_SOURCES variable
        Move the file to the ENGINE_SOURCES variable in CMakeLists.txt")
    endif ()
endforeach ()

add_library(MegaEngineSDK_shared SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_sources(MegaEngineSDK_shared PUBLIC FILE_SET engine_headers TYPE HEADERS FILES ${ENGINE_HEADERS})

target_compile_definitions(MegaEngineSDK_shared PRIVATE EDITOR_SDK)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(MegaEngineSDK_shared INTERFACE glfw)

find_package(sigc++-3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")

target_precompile_headers(MegaEngineSDK_shared
        PRIVATE common.h
)
target_include_directories(
        MegaEngineSDK_shared ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/include>"
)
target_include_directories(
        MegaEngineSDK_shared ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
)

target_link_libraries(MegaEngineSDK_shared PRIVATE glm::glm)
target_link_libraries(MegaEngineSDK_shared PUBLIC stdc++exp MegaEngineUtils)
target_link_libraries(MegaEngineSDK_shared PRIVATE sigc-3.0)
target_include_directories(MegaEngineSDK_shared PRIVATE ${PNGPP_INCLUDE_DIRS})
set_property(TARGET MegaEngineSDK_shared PROPERTY OUTPUT_NAME MegaEngineSDK)
set_property(TARGET MegaEngineSDK_shared PROPERTY PREFIX "")

add_custom_target(MegaEngineSDK)
add_dependencies(MegaEngineSDK MegaEngineSDK_shared MegaEngineUtils)

install(
        TARGETS MegaEngineSDK_shared
        ARCHIVE
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
        COMPONENT sdk
        LIBRARY
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
        COMPONENT sdk
        FILE_SET engine_headers
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/"
        COMPONENT sdk
)


