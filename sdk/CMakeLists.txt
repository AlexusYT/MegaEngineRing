

set(ENGINE_SOURCES sources/context/Application.cpp
        sources/context/DefaultApplicationSettings.cpp
        sources/context/Window.cpp
        sources/context/MainWindow.cpp
        sources/resources/LoadedResources.cpp
        sources/resources/shaders/Shader.cpp
        sources/context/ResourcesWindow.cpp
        sources/resources/ResourceLoader.cpp
        sources/resources/Resources.cpp
        sources/scene/Scene.cpp
        sources/resources/shaders/ShaderProgram.cpp
        sources/resources/shaders/ShaderProgramLoader.cpp
        sources/buffers/ProgramWideShaderBuffer.cpp
        sources/buffers/SSBO.cpp
        sources/scene/objects/SceneObject.cpp
        sources/extensions/Extension.cpp
        sources/extensions/BasicRenderExtension.cpp
        sources/extensions/cameras/CameraExtension.cpp
        sources/extensions/cameras/CameraKeyboardExtension.cpp
        sources/extensions/ExtensionRegistry.cpp
        sources/extensions/cameras/PerspectiveProjectionCameraMod.cpp
        sources/extensions/OrbitCameraExtension.cpp
        sources/resources/models/Model3DLoader.cpp
        sources/extensions/MeshExtension.cpp
        sources/scripting/Script.cpp
        sources/extensions/MainObjectExtension.cpp
        sources/utils/PropertyJsonSerializers.cpp
        sources/resources/models/Model3DResource.cpp
        sources/resources/models/Model3DObject.cpp
        sources/render/RenderInstance.cpp
        sources/resources/FileSystemResourceBundle.cpp
        sources/resources/ResourceLoaders.cpp
        sources/resources/Resource.cpp
        sources/resources/textures/TextureResource.cpp
        sources/resources/textures/TextureLoader.cpp
        sources/extensions/LightExtension.cpp
        sources/light/LightSources.cpp
        sources/resources/materials/MaterialResource.cpp
        sources/resources/materials/MaterialLoader.cpp
        sources/resources/ResourceLoadResult.cpp
        sources/resources/materials/ColorComponent.cpp
        sources/prefabs/Prefab.cpp
        sources/prefabs/PrefabInstance.cpp
        sources/prefabs/elements/PrefabElementInstance.cpp
        sources/prefabs/elements/PrefabElementsSsbo.cpp
        sources/prefabs/elements/PrefabElement.cpp
        sources/prefabs/elements/MeshPrefabElement.cpp
        sources/resources/shaders/builtin/PrefabProgram.cpp
        sources/utils/Transformation.cpp
        sources/utils/Transformable.cpp
        sources/prefabs/PrefabSsbo.cpp
        sources/scene/SceneUi.cpp
        sources/ui/UiWindow.cpp
        sources/ui/UiPopup.cpp
        sources/ui/UiBase.cpp
        sources/resources/shaders/builtin/DefaultProgram.cpp
        sources/scene/Scene3D.cpp
        sources/gltf/Accessor.cpp
        sources/gltf/Accessor.cpp
        sources/gltf/Primitive.cpp
        sources/gltf/GltfModel.cpp
        sources/gltf/Mesh.cpp
        sources/gltf/Node.cpp
        sources/gltf/Sampler.cpp
        sources/gltf/Texture.cpp
        sources/gltf/Image.cpp
        sources/gltf/Material.cpp
        sources/buffers/Framebuffer.cpp
        sources/bounding/BoundingVolume.cpp
        sources/bounding/VolumeAabb.cpp
        sources/resources/shaders/builtin/BoundingVolumeProgram.cpp
        sources/meshes/BlockPlaneMesh.cpp
        sources/render/Renderer.cpp
        sources/buffers/VertexBufferObject.cpp
        sources/buffers/BasicBuffer.cpp
        sources/gltf/khr/KhrLightsPunctual.cpp
        sources/gltf/Light.cpp
)
set(ENGINE_HEADERS include/EngineSDK/context/Application.h
        include/EngineSDK/context/Window.h
        include/EngineSDK/context/MainWindow.h
        include/EngineSDK/resources/LoadedResources.h
        include/EngineSDK/context/ResourcesWindow.h
        include/EngineSDK/context/IContext.h
        include/EngineSDK/context/IWindow.h
        include/EngineSDK/context/IApplicationSettings.h
        include/EngineSDK/context/DefaultApplicationSettings.h
        include/EngineSDK/scene/IScene.h
        include/EngineSDK/resources/shaders/ShaderTypeEnum.h
        include/EngineSDK/resources/ResourceLoader.h
        include/EngineSDK/resources/IResource.h
        include/EngineSDK/resources/Resources.h
        include/EngineSDK/scene/Scene.h
        include/EngineSDK/resources/shaders/ShaderProgram.h
        include/EngineSDK/buffers/SSBO.h
        include/EngineSDK/buffers/BufferUsageEnum.h
        include/EngineSDK/resources/shaders/ShaderProgramLoader.h
        include/EngineSDK/buffers/ProgramWideShaderBuffer.h
        include/EngineSDK/resources/shaders/Shader.h
        include/EngineSDK/scene/objects/ISceneObject.h
        include/EngineSDK/extensions/cameras/ICamera.h
        include/EngineSDK/utils/ModifierKeys.h
        include/EngineSDK/utils/KeyboardKey.h
        include/EngineSDK/scene/objects/SceneObject.h
        include/EngineSDK/extensions/Extension.h
        include/EngineSDK/extensions/BasicRenderExtension.h
        include/EngineSDK/resources/IResourceLoadExecutor.h
        include/EngineSDK/extensions/cameras/CameraExtension.h
        include/EngineSDK/extensions/cameras/CameraKeyboardExtension.h
        include/EngineSDK/extensions/cameras/CameraMouseExtension.h
        include/EngineSDK/extensions/ExtensionRegistry.h
        include/EngineSDK/extensions/cameras/PerspectiveProjectionCameraMod.h
        include/EngineSDK/extensions/cameras/IProjectionCameraMod.h
        include/EngineSDK/extensions/cameras/OrbitCameraExtension.h
        include/EngineSDK/utils/MouseButton.h
        include/EngineSDK/extensions/MouseButtonExtension.h
        include/EngineSDK/resources/models/Model3DLoader.h
        include/EngineSDK/extensions/MeshExtension.h
        include/EngineSDK/context/IApplication.h
        include/EngineSDK/scripting/IScript.h
        include/EngineSDK/scripting/Script.h
        include/EngineSDK/extensions/ExtensionProperty.h
        include/EngineSDK/extensions/MainObjectExtension.h
        include/EngineSDK/utils/PropertyJsonSerializers.h
        include/EngineSDK/utils/PropertyJsonSerializer.h
        include/EngineSDK/resources/models/Model3DResource.h
        include/EngineSDK/render/IRenderInstance.h
        include/EngineSDK/render/RenderInstanceData.h
        include/EngineSDK/resources/models/Model3DObject.h
        include/EngineSDK/resources/models/IModel3DObject.h
        include/EngineSDK/resources/models/IModel3DResource.h
        include/EngineSDK/render/RenderInstance.h
        include/EngineSDK/render/IInstancedRender.h
        include/EngineSDK/resources/IResourceBundle.h
        include/EngineSDK/resources/FileSystemResourceBundle.h
        include/EngineSDK/resources/ResourceLoaders.h
        include/EngineSDK/resources/IResourceLoaders.h
        include/EngineSDK/render/IRenderable.h
        include/EngineSDK/resources/Resource.h
        include/EngineSDK/resources/textures/ITextureResource.h
        include/EngineSDK/resources/textures/TextureResource.h
        include/EngineSDK/resources/textures/TextureMinFilter.h
        include/EngineSDK/resources/textures/TextureMagFilter.h
        include/EngineSDK/resources/textures/TextureBaseInternalFormat.h
        include/EngineSDK/resources/textures/Texture2DImageFormat.h
        include/EngineSDK/resources/textures/Texture2DType.h
        include/EngineSDK/resources/ResourceType.h
        include/EngineSDK/resources/textures/TextureLoader.h
        include/EngineSDK/extensions/LightExtension.h
        include/EngineSDK/light/ILightInstance.h
        include/EngineSDK/light/ILightSources.h
        include/EngineSDK/light/LightInstanceData.h
        include/EngineSDK/light/LightSources.h
        include/EngineSDK/resources/materials/IMaterialResource.h
        include/EngineSDK/resources/materials/MaterialResource.h
        include/EngineSDK/resources/materials/MaterialData.h
        include/EngineSDK/resources/materials/MaterialLoader.h
        include/EngineSDK/resources/ResourceLoadResult.h
        include/EngineSDK/utils/ISerializable.h
        include/EngineSDK/render/IInitializable.h
        include/EngineSDK/render/Initializable.h
        include/EngineSDK/resources/materials/IMaterialComponent.h
        include/EngineSDK/resources/materials/ColorComponent.h
        include/EngineSDK/prefabs/Prefab.h
        include/EngineSDK/prefabs/PrefabInstance.h
        include/EngineSDK/prefabs/elements/PrefabElementInstance.h
        include/EngineSDK/prefabs/elements/PrefabElementsSsbo.h
        include/EngineSDK/prefabs/elements/PrefabElement.h
        include/EngineSDK/prefabs/elements/MeshPrefabElement.h
        include/EngineSDK/prefabs/IPrefabElementInstance.h
        include/EngineSDK/prefabs/IPrefabInstance.h
        include/EngineSDK/resources/shaders/builtin/PrefabProgram.h
        include/EngineSDK/utils/Transformation.h
        include/EngineSDK/utils/Transformable.h
        include/EngineSDK/prefabs/PrefabSsbo.h
        include/EngineSDK/scene/SceneUi.h
        include/EngineSDK/ui/UiWindow.h
        include/EngineSDK/ui/UiPopup.h
        include/EngineSDK/ui/UiBase.h
        include/EngineSDK/resources/shaders/builtin/DefaultProgram.h
        include/EngineSDK/scene/Scene3D.h
        include/EngineSDK/gltf/Accessor.h
        include/EngineSDK/gltf/Accessor.h
        include/EngineSDK/gltf/Primitive.h
        include/EngineSDK/gltf/GltfModel.h
        include/EngineSDK/gltf/Mesh.h
        include/EngineSDK/gltf/Node.h
        include/EngineSDK/gltf/Sampler.h
        include/EngineSDK/gltf/Texture.h
        include/EngineSDK/gltf/Image.h
        include/EngineSDK/gltf/Material.h
        include/EngineSDK/buffers/Framebuffer.h
        include/EngineSDK/bounding/BoundingVolume.h
        include/EngineSDK/bounding/VolumeAabb.h
        include/EngineSDK/resources/shaders/builtin/BoundingVolumeProgram.h
        include/EngineSDK/bounding/DebugGeometry.h
        include/EngineSDK/meshes/BlockPlaneMesh.h
        include/EngineSDK/render/Renderer.h
        include/EngineSDK/buffers/VertexBufferObject.h
        include/EngineSDK/buffers/BasicBuffer.h
        include/EngineSDK/gltf/khr/KhrLightsPunctual.h
        include/EngineSDK/gltf/Light.h
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

add_library(MegaEngineSDK_shared SHARED ${ENGINE_SOURCES} imconfig_custom.cpp ${ENGINE_HEADERS})

target_sources(MegaEngineSDK_shared PUBLIC FILE_SET engine_headers TYPE HEADERS FILES ${ENGINE_HEADERS})

target_compile_definitions(MegaEngineSDK_shared PRIVATE EDITOR_SDK)

target_link_libraries(MegaEngineSDK_shared PRIVATE ImGUI)

pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)
if (LINUX)
    target_link_libraries(MegaEngineSDK_shared PUBLIC glfw)
else ()
    target_link_libraries(MegaEngineSDK_shared PUBLIC glfw3)
endif ()

find_path(PNGPP_INCLUDE_DIRS "png++/color.hpp")

target_precompile_headers(MegaEngineSDK_shared
        PRIVATE common.h
)
target_include_directories(
        MegaEngineSDK_shared ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/include>"
)

target_link_libraries(MegaEngineSDK_shared PUBLIC PkgConfig::epoxy)
target_link_libraries(MegaEngineSDK_shared PUBLIC glm::glm)
target_link_libraries(MegaEngineSDK_shared PUBLIC stdc++exp MegaEngineUtils)
target_include_directories(MegaEngineSDK_shared SYSTEM PUBLIC ${PNGPP_INCLUDE_DIRS})
target_link_libraries(MegaEngineSDK_shared PUBLIC PNG::PNG)
target_link_libraries(MegaEngineSDK_shared PUBLIC GLTFSDK)

set_property(TARGET MegaEngineSDK_shared PROPERTY OUTPUT_NAME MegaEngineSDK)
set_property(TARGET MegaEngineSDK_shared PROPERTY PREFIX "")
target_compile_options(MegaEngineSDK_shared PRIVATE -Werror)

add_custom_target(MegaEngineSDK)
add_dependencies(MegaEngineSDK MegaEngineSDK_shared MegaEngineUtils)

if (WIN32)
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
else ()
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
endif ()

#[[add_custom_command(TARGET MegaEngineSDK_shared POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaEngineSDK_shared> $<TARGET_FILE_DIR:MegaEngineSDK_shared>
        # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:MegaEngineSDK_shared> $<TARGET_FILE_DIR:MegaEngineSDK_test>

        COMMAND_EXPAND_LISTS
)]]
