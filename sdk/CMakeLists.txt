

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
        include/EngineSDK/main/resources/ResourceLoader.cpp
        include/EngineSDK/main/resources/Resources.cpp
        include/EngineSDK/main/scene/Scene.cpp
        include/EngineSDK/renderer/shaders/ShaderProgram.cpp
        include/EngineSDK/main/resources/shaders/ShaderProgramLoader.cpp
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
        include/EngineSDK/main/resources/materials/MaterialLoader.cpp
        include/EngineSDK/main/resources/ResourceLoadResult.cpp
        include/EngineSDK/main/resources/materials/ColorComponent.cpp
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
        include/EngineSDK/main/context/IContext.h
        include/EngineSDK/main/IWindow.h
        include/EngineSDK/main/IApplicationSettings.h
        include/EngineSDK/main/DefaultApplicationSettings.h
        include/EngineSDK/main/scene/IScene.h
        include/EngineSDK/main/resources/ResourceRequest.h
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
        include/EngineSDK/main/resources/shaders/ShaderProgramLoader.h
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
        include/EngineSDK/main/scene/objects/extensions/cameras/OrbitCameraExtension.h
        include/EngineSDK/utils/MouseButton.h
        include/EngineSDK/main/scene/objects/extensions/MouseButtonExtension.h
        include/EngineSDK/main/resources/models/Model3DLoader.h
        include/EngineSDK/main/scene/objects/extensions/ModelRenderExtension.h
        include/EngineSDK/main/IApplication.h
        include/EngineSDK/main/scripting/IScript.h
        include/EngineSDK/main/scripting/Script.h
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
        include/EngineSDK/renderer/shaders/GeometryShader.h
        include/EngineSDK/main/prefabs/PrefabSsbo.h
)

add_library(MegaEngineSDK_shared SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_sources(MegaEngineSDK_shared PUBLIC FILE_SET engine_headers TYPE HEADERS FILES ${ENGINE_HEADERS})

target_compile_definitions(MegaEngineSDK_shared PRIVATE EDITOR_SDK)

pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)
target_link_libraries(MegaEngineSDK_shared PUBLIC glfw3)

find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")

target_precompile_headers(MegaEngineSDK_shared
        PRIVATE common.h
)
target_include_directories(
        MegaEngineSDK_shared ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/include>"
)

target_link_libraries(MegaEngineSDK_shared PUBLIC epoxy)
target_link_libraries(MegaEngineSDK_shared PUBLIC glm::glm)
target_link_libraries(MegaEngineSDK_shared PUBLIC stdc++exp MegaEngineUtils)
target_include_directories(MegaEngineSDK_shared SYSTEM PUBLIC ${PNGPP_INCLUDE_DIRS})
target_link_libraries(MegaEngineSDK_shared PUBLIC PNG::PNG)
set_property(TARGET MegaEngineSDK_shared PROPERTY OUTPUT_NAME MegaEngineSDK)
set_property(TARGET MegaEngineSDK_shared PROPERTY PREFIX "")

add_custom_target(MegaEngineSDK)
add_dependencies(MegaEngineSDK MegaEngineSDK_shared MegaEngineUtils)

if(WIN32)
install(
        TARGETS MegaEngineSDK_shared
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
        POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
        DIRECTORIES
        $<TARGET_FILE_DIR:MegaEngineSDK_shared>
        "C:\\msys64\\mingw64\\bin"
        ARCHIVE
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
        COMPONENT sdk
        RUNTIME
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/"
        COMPONENT sdk
        LIBRARY
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
        COMPONENT sdk
        FILE_SET engine_headers
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/"
        COMPONENT sdk

)
else()
    install(
            TARGETS MegaEngineSDK_shared
            ARCHIVE
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
            COMPONENT sdk
            RUNTIME
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
            COMPONENT sdk
            LIBRARY
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/lib/"
            COMPONENT sdk
            FILE_SET engine_headers
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALSTATEDIR}/${CMAKE_PROJECT_NAME}/sdk/${SDK_VERSION}/"
            COMPONENT sdk
    )
endif()
message(STATUS "$<TARGET_RUNTIME_DLLS:MegaEngineSDK_shared>")

#[[add_custom_command(TARGET MegaEngineSDK_shared POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaEngineSDK_shared> $<TARGET_FILE_DIR:MegaEngineSDK_shared>
        # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaEngineSDK_shared> $<TARGET_FILE_DIR:MegaEngineSDK_test>

        COMMAND_EXPAND_LISTS
)]]
