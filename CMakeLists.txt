cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

# =========================================================
# Project
# =========================================================

project(opengl_project_template LANGUAGES C CXX)

# =========================================================
# C++ settings
# =========================================================

set(CMAKE_CXX_STANDARD            23)
set(CMAKE_CXX_STANDARD_REQUIRED   ON)
set(CMAKE_CXX_EXTENSIONS          OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================================================
# C++ compiler info
# =========================================================

message(STATUS "CXX Compiler ID : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX Simulate ID : ${CMAKE_CXX_SIMULATE_ID}")

# =========================================================
# Compile options
# =========================================================

if(MSVC)
    add_compile_options(/utf-8)
endif()

# =========================================================
# Compile definitions
# =========================================================

add_compile_definitions(
    -DGLM_FORCE_RADIANS
)

if(MSVC)
    add_compile_definitions(
        -D_CRT_SECURE_NO_WARNINGS
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
    )
endif()


# =========================================================
# Paths
# =========================================================

set(REPOSITORY_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(EXTERNAL_DIRECTORY ${REPOSITORY_DIRECTORY}/external)

# =========================================================
# External
# =========================================================

set(GLFW_BUILD_DOCS      OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)

set(SPDLOG_BUILD_SHARED  OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH   OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)

add_subdirectory(${EXTERNAL_DIRECTORY}/glfw)
add_subdirectory(${EXTERNAL_DIRECTORY}/spdlog)

# =========================================================
# Libraries - external
# =========================================================

include(${REPOSITORY_DIRECTORY}/cmake/glad.cmake)
include(${REPOSITORY_DIRECTORY}/cmake/imgui.cmake)
include(${REPOSITORY_DIRECTORY}/cmake/imguizmo.cmake)

# =========================================================
# Libraries
# =========================================================

add_subdirectory(${REPOSITORY_DIRECTORY}/core)

# =========================================================
# Executable
# =========================================================

add_subdirectory(${REPOSITORY_DIRECTORY}/app)
