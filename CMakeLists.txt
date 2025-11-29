cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(opengl-project-template)

get_filename_component(REPOSITORY_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE)
set(EXTERNAL_DIRECTORY ${REPOSITORY_DIRECTORY}/external)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIRECTORY}/glfw)
add_subdirectory(${EXTERNAL_DIRECTORY}/spdlog)

add_executable(opengl-project-template-application
    ${REPOSITORY_DIRECTORY}/app/main.cpp)

target_sources(opengl-project-template-application
    PRIVATE
        ${REPOSITORY_DIRECTORY}/src/Buffer.cpp
        ${REPOSITORY_DIRECTORY}/src/Camera.cpp
        ${REPOSITORY_DIRECTORY}/src/DebugCallbacks.cpp
        ${REPOSITORY_DIRECTORY}/src/Program.cpp
        ${REPOSITORY_DIRECTORY}/src/VertexArray.cpp
        ${EXTERNAL_DIRECTORY}/glad/src/glad.c
        ${EXTERNAL_DIRECTORY}/imgui/imgui.cpp
        ${EXTERNAL_DIRECTORY}/imgui/imgui_demo.cpp
        ${EXTERNAL_DIRECTORY}/imgui/imgui_draw.cpp
        ${EXTERNAL_DIRECTORY}/imgui/imgui_tables.cpp
        ${EXTERNAL_DIRECTORY}/imgui/imgui_widgets.cpp
        ${EXTERNAL_DIRECTORY}/imgui/backends/imgui_impl_opengl3.cpp
        ${EXTERNAL_DIRECTORY}/imgui/backends/imgui_impl_glfw.cpp)

target_link_libraries(opengl-project-template-application
    PRIVATE
        glfw
        spdlog)

target_include_directories(opengl-project-template-application
    PRIVATE
        ${REPOSITORY_DIRECTORY}/include
        ${EXTERNAL_DIRECTORY}/glfw/include
        ${EXTERNAL_DIRECTORY}/glad/include
        ${EXTERNAL_DIRECTORY}/imgui
        ${EXTERNAL_DIRECTORY}/imgui/backends
        ${EXTERNAL_DIRECTORY}/glm)

target_compile_definitions(opengl-project-template-application
    PRIVATE
        -DGLM_ENABLE_EXPERIMENTAL
        -DGLM_FORCE_RADIANS)
