

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
        sources/meshes/BlockCubeMesh.cpp
        sources/meshes/BlockSphereMesh.cpp
        sources/utils/Logger.cpp
        sources/utils/UUID.cpp
        sources/utils/Utils.cpp
        sources/utils/ReportMessage.cpp
        sources/utils/ConsoleColors.cpp
        sources/utils/PropertyBase.cpp
        sources/render/RenderPass.cpp
        sources/render/DefaultRenderer.cpp
)
set(ENGINE_HEADERS include/KwasarEngine/context/Application.h
        include/KwasarEngine/context/Window.h
        include/KwasarEngine/context/MainWindow.h
        include/KwasarEngine/resources/LoadedResources.h
        include/KwasarEngine/context/ResourcesWindow.h
        include/KwasarEngine/context/IContext.h
        include/KwasarEngine/context/IWindow.h
        include/KwasarEngine/context/IApplicationSettings.h
        include/KwasarEngine/context/DefaultApplicationSettings.h
        include/KwasarEngine/scene/IScene.h
        include/KwasarEngine/resources/shaders/ShaderTypeEnum.h
        include/KwasarEngine/resources/ResourceLoader.h
        include/KwasarEngine/resources/IResource.h
        include/KwasarEngine/resources/Resources.h
        include/KwasarEngine/scene/Scene.h
        include/KwasarEngine/resources/shaders/ShaderProgram.h
        include/KwasarEngine/buffers/SSBO.h
        include/KwasarEngine/buffers/BufferUsageEnum.h
        include/KwasarEngine/resources/shaders/ShaderProgramLoader.h
        include/KwasarEngine/buffers/ProgramWideShaderBuffer.h
        include/KwasarEngine/resources/shaders/Shader.h
        include/KwasarEngine/scene/objects/ISceneObject.h
        include/KwasarEngine/extensions/cameras/ICamera.h
        include/KwasarEngine/utils/ModifierKeys.h
        include/KwasarEngine/utils/KeyboardKey.h
        include/KwasarEngine/scene/objects/SceneObject.h
        include/KwasarEngine/extensions/Extension.h
        include/KwasarEngine/extensions/BasicRenderExtension.h
        include/KwasarEngine/resources/IResourceLoadExecutor.h
        include/KwasarEngine/extensions/cameras/CameraExtension.h
        include/KwasarEngine/extensions/cameras/CameraKeyboardExtension.h
        include/KwasarEngine/extensions/cameras/CameraMouseExtension.h
        include/KwasarEngine/extensions/ExtensionRegistry.h
        include/KwasarEngine/extensions/cameras/PerspectiveProjectionCameraMod.h
        include/KwasarEngine/extensions/cameras/IProjectionCameraMod.h
        include/KwasarEngine/extensions/cameras/OrbitCameraExtension.h
        include/KwasarEngine/utils/MouseButton.h
        include/KwasarEngine/extensions/MouseButtonExtension.h
        include/KwasarEngine/resources/models/Model3DLoader.h
        include/KwasarEngine/extensions/MeshExtension.h
        include/KwasarEngine/context/IApplication.h
        include/KwasarEngine/scripting/IScript.h
        include/KwasarEngine/scripting/Script.h
        include/KwasarEngine/extensions/ExtensionProperty.h
        include/KwasarEngine/extensions/MainObjectExtension.h
        include/KwasarEngine/utils/PropertyJsonSerializers.h
        include/KwasarEngine/utils/PropertyJsonSerializer.h
        include/KwasarEngine/resources/models/Model3DResource.h
        include/KwasarEngine/render/IRenderInstance.h
        include/KwasarEngine/render/RenderInstanceData.h
        include/KwasarEngine/resources/models/Model3DObject.h
        include/KwasarEngine/resources/models/IModel3DObject.h
        include/KwasarEngine/resources/models/IModel3DResource.h
        include/KwasarEngine/render/RenderInstance.h
        include/KwasarEngine/render/IInstancedRender.h
        include/KwasarEngine/resources/IResourceBundle.h
        include/KwasarEngine/resources/FileSystemResourceBundle.h
        include/KwasarEngine/resources/ResourceLoaders.h
        include/KwasarEngine/resources/IResourceLoaders.h
        include/KwasarEngine/render/IRenderable.h
        include/KwasarEngine/resources/Resource.h
        include/KwasarEngine/resources/textures/ITextureResource.h
        include/KwasarEngine/resources/textures/TextureResource.h
        include/KwasarEngine/resources/textures/TextureMinFilter.h
        include/KwasarEngine/resources/textures/TextureMagFilter.h
        include/KwasarEngine/resources/textures/TextureBaseInternalFormat.h
        include/KwasarEngine/resources/textures/Texture2DImageFormat.h
        include/KwasarEngine/resources/textures/Texture2DType.h
        include/KwasarEngine/resources/ResourceType.h
        include/KwasarEngine/resources/textures/TextureLoader.h
        include/KwasarEngine/extensions/LightExtension.h
        include/KwasarEngine/light/ILightInstance.h
        include/KwasarEngine/light/ILightSources.h
        include/KwasarEngine/light/LightInstanceData.h
        include/KwasarEngine/light/LightSources.h
        include/KwasarEngine/resources/materials/IMaterialResource.h
        include/KwasarEngine/resources/materials/MaterialResource.h
        include/KwasarEngine/resources/materials/MaterialData.h
        include/KwasarEngine/resources/materials/MaterialLoader.h
        include/KwasarEngine/resources/ResourceLoadResult.h
        include/KwasarEngine/utils/ISerializable.h
        include/KwasarEngine/render/IInitializable.h
        include/KwasarEngine/render/Initializable.h
        include/KwasarEngine/resources/materials/IMaterialComponent.h
        include/KwasarEngine/resources/materials/ColorComponent.h
        include/KwasarEngine/prefabs/Prefab.h
        include/KwasarEngine/prefabs/PrefabInstance.h
        include/KwasarEngine/prefabs/elements/PrefabElementInstance.h
        include/KwasarEngine/prefabs/elements/PrefabElementsSsbo.h
        include/KwasarEngine/prefabs/elements/PrefabElement.h
        include/KwasarEngine/prefabs/elements/MeshPrefabElement.h
        include/KwasarEngine/prefabs/IPrefabElementInstance.h
        include/KwasarEngine/prefabs/IPrefabInstance.h
        include/KwasarEngine/resources/shaders/builtin/PrefabProgram.h
        include/KwasarEngine/utils/Transformation.h
        include/KwasarEngine/utils/Transformable.h
        include/KwasarEngine/prefabs/PrefabSsbo.h
        include/KwasarEngine/scene/SceneUi.h
        include/KwasarEngine/ui/UiWindow.h
        include/KwasarEngine/ui/UiPopup.h
        include/KwasarEngine/ui/UiBase.h
        include/KwasarEngine/resources/shaders/builtin/DefaultProgram.h
        include/KwasarEngine/scene/Scene3D.h
        include/KwasarEngine/gltf/Accessor.h
        include/KwasarEngine/gltf/Accessor.h
        include/KwasarEngine/gltf/Primitive.h
        include/KwasarEngine/gltf/GltfModel.h
        include/KwasarEngine/gltf/Mesh.h
        include/KwasarEngine/gltf/Node.h
        include/KwasarEngine/gltf/Sampler.h
        include/KwasarEngine/gltf/Texture.h
        include/KwasarEngine/gltf/Image.h
        include/KwasarEngine/gltf/Material.h
        include/KwasarEngine/buffers/Framebuffer.h
        include/KwasarEngine/bounding/BoundingVolume.h
        include/KwasarEngine/bounding/VolumeAabb.h
        include/KwasarEngine/resources/shaders/builtin/BoundingVolumeProgram.h
        include/KwasarEngine/bounding/DebugGeometry.h
        include/KwasarEngine/meshes/BlockPlaneMesh.h
        include/KwasarEngine/render/Renderer.h
        include/KwasarEngine/buffers/VertexBufferObject.h
        include/KwasarEngine/buffers/BasicBuffer.h
        include/KwasarEngine/gltf/khr/KhrLightsPunctual.h
        include/KwasarEngine/gltf/Light.h
        include/KwasarEngine/meshes/BlockCubeMesh.h
        include/KwasarEngine/meshes/BlockSphereMesh.h
        include/KwasarEngine/utils/Logger.h
        include/KwasarEngine/utils/UUID.h
        include/KwasarEngine/utils/Utils.h
        include/KwasarEngine/utils/ReportMessageFwd.h
        include/KwasarEngine/utils/IPropertyProvider.h
        include/KwasarEngine/utils/PropertyBase.h
        include/KwasarEngine/utils/Property.h
        include/KwasarEngine/utils/PropertyReadOnly.h
        include/KwasarEngine/utils/IReportable.h
        include/KwasarEngine/utils/PropertyNotifiable.h
        include/KwasarEngine/render/RenderPass.h
        include/KwasarEngine/render/DrawElementsIndirectCommand.h
        include/KwasarEngine/render/DefaultRenderer.h
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

add_library(KwasarEngineLib STATIC ${ENGINE_SOURCES} imconfig_custom.cpp ${ENGINE_HEADERS})

target_sources(KwasarEngineLib PUBLIC FILE_SET engine_headers TYPE HEADERS BASE_DIRS "include" FILES ${ENGINE_HEADERS})

target_compile_definitions(KwasarEngineLib PUBLIC GLM_ENABLE_EXPERIMENTAL)
target_compile_definitions(KwasarEngineLib PRIVATE EDITOR_SDK)

target_link_libraries(KwasarEngineLib PRIVATE ImGUI)

pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)
if (LINUX)
    target_link_libraries(KwasarEngineLib PUBLIC glfw)
else ()
    target_link_libraries(KwasarEngineLib PUBLIC glfw3)
endif ()

target_precompile_headers(KwasarEngineLib
        PRIVATE common.h
)
target_include_directories(
        KwasarEngineLib ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/engine/include>"
)

target_link_libraries(KwasarEngineLib PUBLIC PkgConfig::epoxy)
target_link_libraries(KwasarEngineLib PUBLIC glm::glm)
target_link_libraries(KwasarEngineLib PUBLIC stdc++exp)
target_link_libraries(KwasarEngineLib PUBLIC spng)
target_link_libraries(KwasarEngineLib PUBLIC GLTFSDK)
target_include_directories(KwasarEngineLib SYSTEM PUBLIC ${sigc++-3_INCLUDE_DIRS})
target_link_libraries(KwasarEngineLib PUBLIC ${sigc++-3_LDFLAGS})
target_link_libraries(KwasarEngineLib PUBLIC turbojpeg)
target_include_directories(KwasarEngineLib SYSTEM PUBLIC ${libturbojpeg_INCLUDE_DIRS})

set_property(TARGET KwasarEngineLib PROPERTY OUTPUT_NAME KwasarEngine)
target_compile_options(KwasarEngineLib PRIVATE -Werror)


if (WIN32)
    install(
            TARGETS KwasarEngineLib
            RUNTIME_DEPENDENCIES
            PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
            POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
            DIRECTORIES
            $<TARGET_FILE_DIR:KwasarEngineLib>
            ARCHIVE
            COMPONENT engine
            RUNTIME
            COMPONENT engine
            LIBRARY
            COMPONENT engine
            FILE_SET engine_headers
            COMPONENT engine

    )
else ()
    install(
            TARGETS KwasarEngineLib
            ARCHIVE
            COMPONENT engine
            RUNTIME
            COMPONENT engine
            LIBRARY
            COMPONENT engine
            FILE_SET engine_headers
            COMPONENT engine
    )
endif ()

