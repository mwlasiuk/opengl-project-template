#define USE_LEGACY_OPENGL 0

#include <array>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

// clang-format off
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
// clang-format on

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "DebugCallbacks.hpp"
#include "Structures.hpp"

#if !USE_LEGACY_OPENGL
    #include "Buffer.hpp"
    #include "DefaultShaders.hpp"
    #include "DefaultVertexLayouts.hpp"
    #include "Program.hpp"
    #include "VertexArray.hpp"
    #include "VertexBufferLayout.hpp"
#endif

struct WindowUserData
{
    CameraInputState input  = {};
    Camera*          camera = {};

    int32_t window_iconified      = {};
    int32_t window_maximized      = {};
    int32_t window_focused        = {};
    int32_t window_cursor_entered = {};

    int32_t framebuffer_width  = {};
    int32_t framebuffer_height = {};
};

static void window_position_callback(GLFWwindow* window, const int32_t xpos, const int32_t ypos)
{
}

static void window_size_callback(GLFWwindow* window, const int32_t width, const int32_t height)
{
}

static void window_close_callback(GLFWwindow* window)
{
}

static void window_refresh_callback(GLFWwindow* window)
{
}

static void window_focus_callback(GLFWwindow* window, const int32_t focused)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    data->window_focused = focused;
}

static void window_iconify_callback(GLFWwindow* window, const int32_t iconified)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    data->window_iconified = iconified;
}

static void window_maximize_callback(GLFWwindow* window, const int32_t maximized)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    data->window_maximized = maximized;
}

static void framebuffer_size_callback(GLFWwindow* window, const int32_t width, const int32_t height)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    data->framebuffer_width  = width;
    data->framebuffer_height = height;
}

static void window_content_scale_callback(GLFWwindow* window, const float xscale, const float yscale)
{
}

static void key_callback(GLFWwindow* window, const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    if (key == GLFW_KEY_R)
    {
        data->camera->ResetTarget();
    }
}

static void char_callbacks(GLFWwindow* window, const uint32_t codepoint)
{
}

static void char_mods_callback(GLFWwindow* window, const uint32_t codepoint, const int32_t mods)
{
}

static void mouse_button_callback(GLFWwindow* window, const int32_t button, const int32_t action, const int32_t mods)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        data->input.mouse_pressed_left = (action == GLFW_PRESS);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        data->input.mouse_pressed_right = (action == GLFW_PRESS);
    }

    if (action == GLFW_RELEASE)
    {
        data->input.is_first_mouse = true;
    }
}

static void cursor_position_callback(GLFWwindow* window, const double xpos, const double ypos)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    if (data->input.is_first_mouse)
    {
        data->input.is_first_mouse = false;

        data->input.last_mouse_position_x = xpos;
        data->input.last_mouse_position_y = ypos;
    }

    const float delta_x = (float)(xpos - data->input.last_mouse_position_x);
    const float delta_y = (float)(data->input.last_mouse_position_y - ypos);

    data->input.last_mouse_position_x = xpos;
    data->input.last_mouse_position_y = ypos;

    if (data->input.mouse_pressed_left)
    {
        data->camera->Orbit(delta_x, delta_y);
    }

    if (data->input.mouse_pressed_right)
    {
        data->camera->Pan(delta_x, delta_y);
    }
}

static void cursor_enter_callback(GLFWwindow* window, const int32_t entered)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    data->window_cursor_entered = entered;
}

static void scroll_callback(GLFWwindow* window, const double xoffset, const double yoffset)
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (data)
    {
        data->camera->Zoom((float)yoffset);
    }
}

static void drop_callback(GLFWwindow* window, const int32_t path_count, const char** paths)
{
    spdlog::info("Drop {} : {}", static_cast<void*>(window), path_count);

    for (int32_t i = 0; i < path_count; i++)
    {
        spdlog::info(" - {} : {}", i, paths[i]);
    }
}

static void joystick_callback(const int32_t joystick_id, const int32_t event)
{
}

static void monitorr_callback(GLFWmonitor* monitor, const int32_t event)
{
}

static void register_callbacks(GLFWwindow* window)
{
    glfwSetWindowPosCallback(window, window_position_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetWindowMaximizeCallback(window, window_maximize_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowContentScaleCallback(window, window_content_scale_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, char_callbacks);
    glfwSetCharModsCallback(window, char_mods_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetDropCallback(window, drop_callback);

    glfwSetJoystickCallback(joystick_callback);
    glfwSetMonitorCallback(monitorr_callback);
}

int main()
{
    glfwSetErrorCallback(DebugCallbacks::glfw_error_callback);

    if (!glfwInit())
    {
        spdlog::critical("Failed to initialize GLFW!");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

#if !USE_LEGACY_OPENGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "opengl-project-template-application", nullptr, nullptr);

    if (!window)
    {
        spdlog::critical("Failed to create GLFW window!");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    Camera camera = {};

    WindowUserData window_user_data        = {};
    window_user_data.input                 = {};
    window_user_data.camera                = &camera;
    window_user_data.window_iconified      = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
    window_user_data.window_maximized      = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
    window_user_data.window_focused        = glfwGetWindowAttrib(window, GLFW_FOCUSED);
    window_user_data.window_cursor_entered = glfwGetWindowAttrib(window, GLFW_HOVERED);
    glfwGetFramebufferSize(window, &window_user_data.framebuffer_width, &window_user_data.framebuffer_height);

    glfwSetWindowUserPointer(window, &window_user_data);

    register_callbacks(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallbacks::opengl_error_callback, nullptr);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spdlog::info("OpenGL version : {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    spdlog::info("OpenGL vendor : {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    spdlog::info("OpenGL renderer : {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    spdlog::info("OpenGL GLSL version : {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        spdlog::critical("Failed to initialize ImGui for GLFW!");
        return EXIT_FAILURE;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 450 core"))
    {
        spdlog::critical("Failed to initialize ImGui for OpenGL!");
        return EXIT_FAILURE;
    }

    const std::array<ColorPoint3D, 6> origin_points = {
        ColorPoint3D{{0.0f, 0.0f, 0.0f}, {0xFF, 0x00, 0x00, 0xFF}},
        ColorPoint3D{{1.0f, 0.0f, 0.0f}, {0xFF, 0x00, 0x00, 0xFF}},
        ColorPoint3D{{0.0f, 0.0f, 0.0f}, {0x00, 0xFF, 0x00, 0xFF}},
        ColorPoint3D{{0.0f, 1.0f, 0.0f}, {0x00, 0xFF, 0x00, 0xFF}},
        ColorPoint3D{{0.0f, 0.0f, 0.0f}, {0x00, 0x00, 0xFF, 0xFF}},
        ColorPoint3D{{0.0f, 0.0f, 1.0f}, {0x00, 0x00, 0xFF, 0xFF}}};

    const std::array<Point3D, 6> target_points = {
        Point3D{{-0.1f, 0.0f, 0.0f}},
        Point3D{{0.1f, 0.0f, 0.0f}},
        Point3D{{0.0f, -0.1f, 0.0f}},
        Point3D{{0.0f, 0.1f, 0.0f}},
        Point3D{{0.0f, 0.0f, -0.1f}},
        Point3D{{0.0f, 0.0f, 0.1f}}};

    const std::array<Point2D, 4> plane_points = {
        Point2D{{-1.0f, -1.0f}},
        Point2D{{1.0f, -1.0f}},
        Point2D{{-1.0f, 1.0f}},
        Point2D{{1.0f, 1.0f}}};

#if !USE_LEGACY_OPENGL
    const ProgramDescriptor program_color_point_descriptor = {
        ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                         .source_size = static_cast<int32_t>(kDefaultShader_ColorPoint_Vertex.length()),
                         .source      = kDefaultShader_ColorPoint_Vertex.c_str()},
        ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                         .source_size = static_cast<int32_t>(kDefaultShader_ColorPoint_Fragment.length()),
                         .source      = kDefaultShader_ColorPoint_Fragment.c_str()}};

    const ProgramDescriptor program_point_descriptor = {
        ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                         .source_size = static_cast<int32_t>(kDefaultShader_Point_Vertex.length()),
                         .source      = kDefaultShader_Point_Vertex.c_str()},
        ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                         .source_size = static_cast<int32_t>(kDefaultShader_Point_Fragment.length()),
                         .source      = kDefaultShader_Point_Fragment.c_str()}};

    const ProgramDescriptor program_grid_descriptor = {
        ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                         .source_size = static_cast<int32_t>(kDefaultShader_Grid_Vertex.length()),
                         .source      = kDefaultShader_Grid_Vertex.c_str()},
        ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                         .source_size = static_cast<int32_t>(kDefaultShader_Grid_Fragment.length()),
                         .source      = kDefaultShader_Grid_Fragment.c_str()}};

    Buffer* buffer_system_transform_uniforms = new Buffer(GL_DYNAMIC_STORAGE_BIT, sizeof(SystemTransformUniforms), nullptr);

    Buffer*      vertex_buffer_origin = new Buffer(GL_NONE, origin_points.size() * sizeof(ColorPoint3D), origin_points.data());
    VertexArray* vertex_array_origin  = new VertexArray(vertex_buffer_origin, false, nullptr, false, DefaultVertexLayouts::GetVertexLayout<ColorPoint3D>());

    Buffer*      vertex_buffer_target = new Buffer(GL_NONE, target_points.size() * sizeof(Point3D), target_points.data());
    VertexArray* vertex_array_target  = new VertexArray(vertex_buffer_target, false, nullptr, false, DefaultVertexLayouts::GetVertexLayout<Point3D>());

    Buffer*      vertex_buffer_grid = new Buffer(GL_NONE, plane_points.size() * sizeof(Point2D), plane_points.data());
    VertexArray* vertex_array_grid  = new VertexArray(vertex_buffer_grid, false, nullptr, false, DefaultVertexLayouts::GetVertexLayout<Point2D>());

    Program* program_color_point = new Program(program_color_point_descriptor);
    Program* program_point       = new Program(program_point_descriptor);
    Program* program_grid        = new Program(program_grid_descriptor);
#endif

    glClearColor(0.35f, 0.15f, 0.25f, 1.0f);
    glClearDepth(1.0f);

    bool show_demo_window = false;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (window_user_data.window_iconified)
        {
            glfwWaitEvents();
            continue;
        }

        glViewport(0, 0, window_user_data.framebuffer_width, window_user_data.framebuffer_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 projection = glm::perspectiveFov(
            glm::radians(60.0f),
            static_cast<float>(window_user_data.framebuffer_width),
            static_cast<float>(window_user_data.framebuffer_height),
            0.001f,
            1000.0f);

        const glm::mat4 view   = camera.GetViewMatrix();
        const glm::vec3 target = camera.GetTarget();

#if USE_LEGACY_OPENGL
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view));

        {
            glDisable(GL_LIGHTING);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);

            constexpr int32_t minor_per_major    = 10;
            constexpr float   grid_spacing_major = 1.0f;
            constexpr float   grid_spacing_minor = grid_spacing_major / static_cast<float>(minor_per_major);
            constexpr int32_t line_count         = 100;
            constexpr float   fade_start         = 20.0f;
            constexpr float   fase_end           = 80.0f;

            const glm::vec4 color_minor = glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
            const glm::vec4 color_major = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            glm::vec3 up   = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 dir1 = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 dir2 = glm::vec3(0.0f, 0.0f, 0.0f);

            switch (window_user_data.camera->GetUpAxis())
            {
            case Camera::UpAxis::X:
                {
                    up   = glm::vec3(1.0f, 0.0f, 0.0f);
                    dir1 = glm::vec3(0.0f, 1.0f, 0.0f);
                    dir2 = glm::vec3(0.0f, 0.0f, 1.0f);
                    break;
                }
            case Camera::UpAxis::Y:
                {
                    up   = glm::vec3(0.0f, 1.0f, 0.0f);
                    dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
                    dir2 = glm::vec3(0.0f, 0.0f, 1.0f);
                    break;
                }
            case Camera::UpAxis::Z:
                {
                    up   = glm::vec3(0.0f, 0.0f, 1.0f);
                    dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
                    dir2 = glm::vec3(0.0f, 1.0f, 0.0f);
                    break;
                }
            }

            glm::vec3 center = target - glm::dot(target, up) * up;

            center -= glm::mod(center, grid_spacing_major);

            glBegin(GL_LINES);
            for (int32_t i = -line_count; i <= line_count; ++i)
            {
                const float offset        = static_cast<float>(i) * grid_spacing_minor;
                const bool  is_major_axis = (i % minor_per_major) == 0;

                const glm::vec3 center_offset_dir_1 = center + offset * dir1;
                const glm::vec3 center_offset_dir_2 = center + offset * dir2;

                const glm::vec3 spacing_minor_dir_1 = grid_spacing_minor * dir1;
                const glm::vec3 spacing_minor_dir_2 = grid_spacing_minor * dir2;

                const glm::vec3 line_spacing_1 = static_cast<float>(line_count) * spacing_minor_dir_1;
                const glm::vec3 line_spacing_2 = static_cast<float>(line_count) * spacing_minor_dir_2;

                // Line along dir1
                const glm::vec3 start1 = center_offset_dir_1 - line_spacing_2;
                const glm::vec3 end1   = center_offset_dir_1 + line_spacing_2;
                const float     alpha1 = glm::clamp(1.0f - glm::length((start1 + end1) * 0.5f - target) / fase_end, 0.0f, 1.0f);

                // Line along dir2
                const glm::vec3 start2 = center_offset_dir_2 - line_spacing_1;
                const glm::vec3 end2   = center_offset_dir_2 + line_spacing_1;
                const float     alpha2 = glm::clamp(1.0f - glm::length((start2 + end2) * 0.5f - target) / fase_end, 0.0f, 1.0f);

                const glm::vec4 axis_color = is_major_axis ? color_major : color_minor;
                const glm::vec4 color1     = axis_color * glm::vec4(1.0f, 1.0f, 1.0f, alpha1);
                const glm::vec4 color2     = axis_color * glm::vec4(1.0f, 1.0f, 1.0f, alpha2);

                glColor4fv(glm::value_ptr(color1));
                glVertex3fv(glm::value_ptr(start1));
                glVertex3fv(glm::value_ptr(end1));

                glColor4fv(glm::value_ptr(color2));
                glVertex3fv(glm::value_ptr(start2));
                glVertex3fv(glm::value_ptr(end2));
            }
            glEnd();

            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }

        glBegin(GL_LINES);
        for (const auto& point : origin_points)
        {
            glColor4ubv(glm::value_ptr(point.color));
            glVertex3fv(glm::value_ptr(point.position));
        }
        glEnd();

        glBegin(GL_LINES);
        for (const auto& point : target_points)
        {
            const glm::vec3 target_shift = target + point.position;

            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3fv(glm::value_ptr(target_shift));
        }
        glEnd();
#else
        SystemTransformUniforms system_transform_uniforms = {};
        system_transform_uniforms.preview_projection      = projection;
        system_transform_uniforms.preview_view            = view;

        buffer_system_transform_uniforms->Upload(&system_transform_uniforms, sizeof(SystemTransformUniforms));
        buffer_system_transform_uniforms->SetAsShaderResource(GL_UNIFORM_BUFFER, 0, sizeof(SystemTransformUniforms), 0);

        program_grid->PushUniformS32("uniform_CameraUpAxis", static_cast<int32_t>(window_user_data.camera->GetUpAxis()));
        program_color_point->PushUniform16F32("uniform_Pose", glm::translate(glm::identity<glm::mat4>(), glm::zero<glm::vec3>()));
        program_point->PushUniform16F32("uniform_Pose", glm::translate(glm::identity<glm::mat4>(), target));
        program_point->PushUniform4F32("uniform_Color", {1.0f, 1.0f, 1.0f, 1.0f});

        glDepthMask(GL_FALSE);
        program_grid->Bind();
        vertex_array_grid->Bind();
        vertex_array_grid->DrawArray(GL_TRIANGLE_STRIP, 4);
        vertex_array_grid->Unbind();
        program_grid->Unbind();
        glDepthMask(GL_TRUE);

        program_color_point->Bind();
        vertex_array_origin->Bind();
        vertex_array_origin->DrawArray(GL_LINES, 6);
        vertex_array_origin->Unbind();
        program_color_point->Unbind();

        program_point->Bind();
        vertex_array_target->Bind();
        vertex_array_target->DrawArray(GL_LINES, 6);
        vertex_array_target->Unbind();
        program_point->Unbind();
#endif

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();

        ImGui::NewFrame();

        if (ImGui::Begin("user_data"))
        {
            if (ImGui::TreeNode("input"))
            {
                ImGui::Text("is_first_mouse        : %s", window_user_data.input.is_first_mouse ? "Y" : "N");
                ImGui::Text("last_mouse_position_x : %.2lf", window_user_data.input.last_mouse_position_x);
                ImGui::Text("last_mouse_position_y : %.2lf", window_user_data.input.last_mouse_position_y);
                ImGui::Text("mouse_pressed_left    : %s", window_user_data.input.mouse_pressed_left ? "Y" : "N");
                ImGui::Text("mouse_pressed_right   : %s", window_user_data.input.mouse_pressed_right ? "Y" : "N");

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("camera"))
            {
                if (!window_user_data.camera)
                {
                    ImGui::Text("camera : null");
                }
                else if (ImGui::TreeNode("camera"))
                {
                    const auto& position = window_user_data.camera->GetPosition();
                    const auto& target   = window_user_data.camera->GetTarget();
                    const auto& up       = window_user_data.camera->GetUp();

                    ImGui::Text("position : [%.2f, %.2f, %.2f]", position.x, position.y, position.z);
                    ImGui::Text("target   : [%.2f, %.2f, %.2f]", target.x, target.y, target.z);
                    ImGui::Text("up       : [%.2f, %.2f, %.2f]", up.x, up.y, up.z);
                    ImGui::Text("yaw      : [%.2f]", window_user_data.camera->GetYaw());
                    ImGui::Text("pitch    : [%.2f]", window_user_data.camera->GetPitch());
                    ImGui::Text("distance : [%.2f]", window_user_data.camera->GetDistance());

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }

            ImGui::Text("window_iconified : %s", window_user_data.window_iconified ? "Y" : "N");
            ImGui::Text("window_maximized : %s", window_user_data.window_maximized ? "Y" : "N");
            ImGui::Text("window_focused : %s", window_user_data.window_focused ? "Y" : "N");
            ImGui::Text("window_cursor_entered : %s", window_user_data.window_cursor_entered ? "Y" : "N");
            ImGui::Text("framebuffer_width : %d", window_user_data.framebuffer_width);
            ImGui::Text("framebuffer_height : %d", window_user_data.framebuffer_height);
        }
        ImGui::End();

        if (ImGui::Begin("settings"))
        {
            ImGui::Checkbox("show_demo_window", &show_demo_window);

            if (ImGui::Button("Set camera UP X"))
            {
                camera.SetUpAxis(Camera::UpAxis::X);
            }

            if (ImGui::Button("Set camera UP Y"))
            {
                camera.SetUpAxis(Camera::UpAxis::Y);
            }

            if (ImGui::Button("Set camera UP Z"))
            {
                camera.SetUpAxis(Camera::UpAxis::Z);
            }
        }
        ImGui::End();

        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        ImGui::EndFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

#if !USE_LEGACY_OPENGL
    delete program_grid;
    delete program_point;
    delete vertex_array_grid;
    delete vertex_buffer_grid;
    delete program_color_point;
    delete vertex_array_target;
    delete vertex_buffer_target;
    delete vertex_array_origin;
    delete vertex_buffer_origin;
    delete buffer_system_transform_uniforms;
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
