

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
        include/EngineSDK/main/scene/TestScene.cpp
        include/EngineSDK/main/scene/TestScene.h
        include/EngineSDK/main/resources/ResourceRequest.cpp
        include/EngineSDK/main/resources/shaders/BuiltInShaderRequest.cpp
        include/EngineSDK/main/resources/ResourceLoader.cpp
        include/EngineSDK/main/resources/ResourceRequests.cpp
        include/EngineSDK/main/resources/Resources.cpp
        include/EngineSDK/main/resources/LazyResource.cpp
        include/EngineSDK/main/scene/Scene.cpp
        include/EngineSDK/renderer/shaders/ShaderProgram.cpp
        include/EngineSDK/main/resources/shaders/BuiltInProgramRequest.cpp
        include/EngineSDK/main/buffers/ProgramWideShaderBuffer.cpp
        include/EngineSDK/main/scene/objects/SceneObject.cpp
        include/EngineSDK/main/scene/objects/BasicRenderObject.cpp
        include/EngineSDK/renderer/GL.cpp
)
set(ENGINE_HEADERS include/EngineSDK/main/Application.h
        include/EngineSDK/main/IApplication.h
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
        include/EngineSDK/main/resources/ResourceRequests.h
        include/EngineSDK/main/resources/IResource.h
        include/EngineSDK/main/resources/Resources.h
        include/EngineSDK/main/resources/LazyResource.h
        include/EngineSDK/main/scene/Scene.h
        include/EngineSDK/renderer/shaders/IShaderProgram.h
        include/EngineSDK/renderer/shaders/ShaderProgram.h
        include/EngineSDK/renderer/buffers/ISSBO.h
        include/EngineSDK/renderer/buffers/SSBO.cpp
        include/EngineSDK/renderer/buffers/SSBO.h
        include/EngineSDK/renderer/buffers/BufferUsageEnum.h
        include/EngineSDK/main/resources/shaders/BuiltInProgramRequest.h
        include/EngineSDK/main/buffers/ProgramWideShaderBuffer.h
        include/EngineSDK/renderer/shaders/IShader.h
        include/EngineSDK/renderer/shaders/Shader.h
        include/EngineSDK/main/scene/objects/ISceneObject.h
        include/EngineSDK/main/scene/objects/SceneObject.h
        include/EngineSDK/main/scene/objects/BasicRenderObject.h
        include/EngineSDK/renderer/GL.h
)

add_library(MegaEngineSDK_shared SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})
add_library(MegaEngineSDK_static STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_sources(MegaEngineSDK_shared PUBLIC FILE_SET engine_headers TYPE HEADERS FILES ${ENGINE_HEADERS})

target_compile_definitions(MegaEngineSDK_shared PRIVATE EDITOR_SDK)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(MegaEngineSDK_static INTERFACE glfw)

find_package(sigc++-3 CONFIG REQUIRED)
function(setupTarget target)
    target_precompile_headers(${target}
            PRIVATE common.h
    )
    target_include_directories(
            ${target} ${warning_guard}
            PUBLIC
            "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/sdk/include>"
    )
    target_include_directories(
            ${target} ${warning_guard}
            PUBLIC
            "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/utils/include>"
    )

    target_link_libraries(${target} PUBLIC stdc++exp MegaEngineUtils)
    target_link_libraries(${target} PRIVATE sigc-3.0)
    set_property(TARGET ${target} PROPERTY OUTPUT_NAME MegaEngineSDK)
    set_property(TARGET ${target} PROPERTY PREFIX "")

endfunction()
setupTarget(MegaEngineSDK_shared)
setupTarget(MegaEngineSDK_static)

add_custom_target(MegaEngineSDK)
add_dependencies(MegaEngineSDK MegaEngineSDK_static MegaEngineSDK_shared MegaEngineUtils)

install(
        TARGETS MegaEngineSDK_static MegaEngineSDK_shared
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


