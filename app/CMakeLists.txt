cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(APP_SOURCE_FILES
    ${REPOSITORY_DIRECTORY}/app/src/ImGUINodes.cpp
    ${REPOSITORY_DIRECTORY}/app/src/ImGUIStyle.cpp
    ${REPOSITORY_DIRECTORY}/app/src/main.cpp
)

set(APP_HEADER_FILES
    ${REPOSITORY_DIRECTORY}/app/include/App/ImGUINodes.hpp
    ${REPOSITORY_DIRECTORY}/app/include/App/ImGUIStyle.hpp
    ${REPOSITORY_DIRECTORY}/app/include/App/Structures.hpp
)

add_executable(app)

target_sources(app
    PRIVATE
        ${APP_SOURCE_FILES}
        ${APP_HEADER_FILES}
)

target_link_libraries(app
    PRIVATE
        glfw
        spdlog::spdlog_header_only
        glad
        imgui
        imguizmo
        core
)

target_include_directories(app
    PRIVATE
        ${REPOSITORY_DIRECTORY}/app/include)
