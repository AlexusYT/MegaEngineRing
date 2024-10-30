

set(ENGINE_SOURCES sources/main/Application.cpp
        sources/main/DefaultApplicationSettings.cpp
        include/EngineSDK/main/context/Window.cpp
        include/EngineSDK/main/context/Window.h
        include/EngineSDK/main/context/MainWindow.cpp
        include/EngineSDK/main/context/MainWindow.h
        include/EngineSDK/main/resources/LoadedResources.cpp
        include/EngineSDK/main/resources/LoadedResources.h
        include/EngineSDK/renderer/shaders/Shader.cpp
        include/EngineSDK/main/context/ResourcesWindow.cpp
        include/EngineSDK/main/context/ResourcesWindow.h
        include/EngineSDK/main/resources/ResourceRequest.cpp
        include/EngineSDK/main/resources/shaders/BuiltInShaderRequest.cpp
        include/EngineSDK/main/resources/ResourceLoader.cpp
        include/EngineSDK/main/resources/Resources.cpp
        include/EngineSDK/main/scene/Scene.cpp
        include/EngineSDK/renderer/shaders/ShaderProgram.cpp
        include/EngineSDK/main/resources/shaders/BuiltInProgramRequest.cpp
        include/EngineSDK/main/buffers/ProgramWideShaderBuffer.cpp
        include/EngineSDK/renderer/GL.cpp
        include/EngineSDK/renderer/buffers/SSBO.cpp
        include/EngineSDK/main/scene/objects/SceneObject.cpp
        sources/main/scene/objects/extensions/Extension.cpp
        sources/main/scene/objects/extensions/BasicRenderExtension.cpp
        sources/main/scene/objects/extensions/cameras/CameraExtension.cpp
        sources/main/scene/objects/extensions/cameras/CameraKeyboardExtension.cpp
        sources/main/scene/objects/extensions/ExtensionRegistry.cpp
        sources/main/scene/objects/extensions/cameras/PerspectiveProjectionCameraMod.cpp
        sources/main/scene/objects/extensions/OrbitCameraExtension.cpp
        sources/main/resources/models/Model3DLoader.cpp
        sources/main/scene/objects/extensions/ModelRenderExtension.cpp
        sources/main/scripting/Script.cpp
        sources/main/scene/objects/extensions/MainObjectExtension.cpp
        sources/utils/PropertyJsonSerializers.cpp
        include/EngineSDK/main/resources/models/Model3DResource.cpp
        include/EngineSDK/main/resources/models/Model3DObject.cpp
        include/EngineSDK/main/render/RenderInstance.cpp
        include/EngineSDK/main/resources/FileSystemResourceBundle.cpp
        include/EngineSDK/main/resources/ResourceLoaders.cpp
        include/EngineSDK/main/resources/Resource.cpp
        sources/main/resources/textures/TextureResource.cpp
        include/EngineSDK/main/resources/textures/TextureLoader.cpp
        include/EngineSDK/main/scene/objects/extensions/LightExtension.cpp
        include/EngineSDK/main/light/LightInstance.cpp
        include/EngineSDK/main/light/LightSources.cpp
        include/EngineSDK/main/resources/materials/MaterialResource.cpp
)
set(ENGINE_HEADERS include/EngineSDK/main/Application.h
        include/EngineSDK/main/context/IContext.h
        include/EngineSDK/main/IWindow.h
        include/EngineSDK/main/IApplicationSettings.h
        include/EngineSDK/main/DefaultApplicationSettings.h
        include/EngineSDK/utils/Property.h
        include/EngineSDK/main/scene/IScene.h
        include/EngineSDK/main/resources/ResourceRequest.h
        include/EngineSDK/main/resources/shaders/BuiltInShaderRequest.h
        include/EngineSDK/renderer/shaders/ShaderTypeEnum.h
        include/EngineSDK/renderer/shaders/VertexShader.h
        include/EngineSDK/renderer/shaders/FragmentShader.h
        include/EngineSDK/main/resources/ResourceLoader.h
        include/EngineSDK/main/resources/IResource.h
        include/EngineSDK/main/resources/Resources.h
        include/EngineSDK/main/scene/Scene.h
        include/EngineSDK/renderer/shaders/IShaderProgram.h
        include/EngineSDK/renderer/shaders/ShaderProgram.h
        include/EngineSDK/renderer/buffers/ISSBO.h
        include/EngineSDK/renderer/buffers/SSBO.h
        include/EngineSDK/renderer/buffers/BufferUsageEnum.h
        include/EngineSDK/main/resources/shaders/BuiltInProgramRequest.h
        include/EngineSDK/main/buffers/ProgramWideShaderBuffer.h
        include/EngineSDK/renderer/shaders/IShader.h
        include/EngineSDK/renderer/shaders/Shader.h
        include/EngineSDK/main/scene/objects/ISceneObject.h
        include/EngineSDK/renderer/GL.h
        include/EngineSDK/main/scene/objects/extensions/cameras/ICamera.h
        include/EngineSDK/main/scene/actor/IActor.h
        include/EngineSDK/utils/ModifierKeys.h
        include/EngineSDK/utils/KeyboardKey.h
        include/EngineSDK/main/scene/objects/SceneObject.h
        include/EngineSDK/main/scene/objects/extensions/Extension.h
        include/EngineSDK/main/scene/objects/extensions/BasicRenderExtension.h
        include/EngineSDK/main/resources/IResourceLoadExecutor.h
        include/EngineSDK/main/scene/objects/extensions/cameras/CameraExtension.h
        include/EngineSDK/main/scene/objects/extensions/cameras/CameraKeyboardExtension.h
        include/EngineSDK/main/scene/objects/extensions/cameras/CameraMouseExtension.h
        include/EngineSDK/main/scene/objects/extensions/ExtensionRegistry.h
        include/EngineSDK/main/scene/objects/extensions/cameras/PerspectiveProjectionCameraMod.h
        include/EngineSDK/main/scene/objects/extensions/cameras/IProjectionCameraMod.h
        include/EngineSDK/main/scene/objects/extensions/ExtensionPropertyBase.h
        include/EngineSDK/main/scene/objects/extensions/cameras/OrbitCameraExtension.h
        include/EngineSDK/utils/MouseButton.h
        include/EngineSDK/main/scene/objects/extensions/MouseButtonExtension.h
        include/EngineSDK/main/resources/models/Model3DLoader.h
        include/EngineSDK/main/scene/objects/extensions/ModelRenderExtension.h
        include/EngineSDK/main/resources/shaders/ShaderProgramRequest.h
        include/EngineSDK/main/IApplication.h
        include/EngineSDK/main/scripting/IScript.h
        include/EngineSDK/main/scripting/Script.h
        include/EngineSDK/main/scene/objects/extensions/IPropertyProvider.h
        include/EngineSDK/main/scene/objects/extensions/ExtensionProperty.h
        include/EngineSDK/main/scene/objects/extensions/MainObjectExtension.h
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
        include/EngineSDK/main/scene/objects/extensions/LightExtension.h
        include/EngineSDK/main/light/ILightInstance.h
        include/EngineSDK/main/light/ILightSources.h
        include/EngineSDK/main/light/LightInstance.h
        include/EngineSDK/main/light/LightInstanceData.h
        include/EngineSDK/main/light/LightSources.h
        include/EngineSDK/main/resources/materials/IMaterialResource.h
        include/EngineSDK/main/resources/materials/MaterialResource.h
)

add_library(MegaEngineSDK_shared SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_sources(MegaEngineSDK_shared PUBLIC FILE_SET engine_headers TYPE HEADERS FILES ${ENGINE_HEADERS})

target_compile_definitions(MegaEngineSDK_shared PRIVATE EDITOR_SDK)
#[[find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(MegaEngineSDK_static INTERFACE glfw)]]

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


