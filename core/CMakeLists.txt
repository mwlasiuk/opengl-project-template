cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CORE_SOURCE_FILES
    ${REPOSITORY_DIRECTORY}/core/src/Buffer.cpp
    ${REPOSITORY_DIRECTORY}/core/src/Camera.cpp
    ${REPOSITORY_DIRECTORY}/core/src/DebugCallbacks.cpp
    ${REPOSITORY_DIRECTORY}/core/src/DefaultShaders.cpp
    ${REPOSITORY_DIRECTORY}/core/src/DefaultVertexLayouts.cpp
    ${REPOSITORY_DIRECTORY}/core/src/DynamicRenderer.cpp
    ${REPOSITORY_DIRECTORY}/core/src/FileIO.cpp
    ${REPOSITORY_DIRECTORY}/core/src/Framebuffer.cpp
    ${REPOSITORY_DIRECTORY}/core/src/GenerateData.cpp
    ${REPOSITORY_DIRECTORY}/core/src/Program.cpp
    ${REPOSITORY_DIRECTORY}/core/src/RayPicking.cpp
    ${REPOSITORY_DIRECTORY}/core/src/Structures.cpp
    ${REPOSITORY_DIRECTORY}/core/src/SystemInfo.cpp
    ${REPOSITORY_DIRECTORY}/core/src/Texture.cpp
    ${REPOSITORY_DIRECTORY}/core/src/VertexArray.cpp
)

set(CORE_HEADER_FILES
    ${REPOSITORY_DIRECTORY}/core/include/Core/Base/NonCopyableNonMovable.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Default/DefaultShaders.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Default/DefaultVertexLayouts.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/OpenGL/Buffer.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/OpenGL/Framebuffer.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/OpenGL/Program.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/OpenGL/Texture.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/OpenGL/VertexArray.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Renderer/DynamicRenderer.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Utils/GenerateData.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Utils/RayPicking.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Camera.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/DebugCallbacks.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/FileIO.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/Structures.hpp
    ${REPOSITORY_DIRECTORY}/core/include/Core/SystemInfo.hpp
)

add_library(core
    STATIC
)

target_sources(core
    PUBLIC
        ${CORE_SOURCE_FILES}
        ${CORE_HEADER_FILES}
)

target_include_directories(core
    PUBLIC
        ${REPOSITORY_DIRECTORY}/core/include/
        ${EXTERNAL_DIRECTORY}/spdlog/include
        ${EXTERNAL_DIRECTORY}/glfw/include
        ${EXTERNAL_DIRECTORY}/glad/include
        ${EXTERNAL_DIRECTORY}/imgui
        ${EXTERNAL_DIRECTORY}/imgui/backends
        ${EXTERNAL_DIRECTORY}/ImGuizmo
        ${EXTERNAL_DIRECTORY}/glm
        ${EXTERNAL_DIRECTORY}/stb
)
