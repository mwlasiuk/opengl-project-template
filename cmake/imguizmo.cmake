include_guard()

set(IMGUIZMO_SOURCE_FILES
    ${EXTERNAL_DIRECTORY}/ImGuizmo/GraphEditor.cpp
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImCurveEdit.cpp
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImGradient.cpp
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImGuizmo.cpp
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImSequencer.cpp
)

set(IMGUIZMO_HEADER_FILES
    ${EXTERNAL_DIRECTORY}/ImGuizmo/GraphEditor.h
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImCurveEdit.h
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImGradient.h
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImGuizmo.h
    ${EXTERNAL_DIRECTORY}/ImGuizmo/ImSequencer.h
)

add_library(imguizmo
    STATIC
)

target_sources(imguizmo
    PUBLIC
        ${IMGUIZMO_SOURCE_FILES}
        ${IMGUIZMO_HEADER_FILES}
)

target_include_directories(imguizmo
    PUBLIC
        ${EXTERNAL_DIRECTORY}/glfw/include
        ${EXTERNAL_DIRECTORY}/imgui
        ${EXTERNAL_DIRECTORY}/imgui/backends
        ${EXTERNAL_DIRECTORY}/ImGuizmo
)
