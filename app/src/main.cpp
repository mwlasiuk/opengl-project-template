#include <array>
#include <fstream>

#include <spdlog/spdlog.h>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

// clang-format off
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
// #include <ImGuizmo.h>
// clang-format on

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Camera.hpp>
#include <Core/DebugCallbacks.hpp>
#include <Core/Default/DefaultShaders.hpp>
#include <Core/Default/DefaultVertexLayouts.hpp>
#include <Core/FileIO.hpp>
#include <Core/OpenGL/Buffer.hpp>
#include <Core/OpenGL/Framebuffer.hpp>
#include <Core/OpenGL/Program.hpp>
#include <Core/OpenGL/Texture.hpp>
#include <Core/OpenGL/VertexArray.hpp>
#include <Core/Renderer/DynamicRenderer.hpp>
#include <Core/Structures.hpp>
#include <Core/SystemInfo.hpp>
#include <Core/Utils/GenerateData.hpp>
#include <Core/Utils/RayPicking.hpp>

#include <App/ImGUINodes.hpp>
#include <App/ImGUIStyle.hpp>
#include <App/Structures.hpp>

using namespace Core;
using namespace App;

static constexpr auto kIdentity = glm::identity<glm::f32mat4x4>();

static void window_position_callback(GLFWwindow* window, const int32_t x_pos, const int32_t y_pos)
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

static void window_content_scale_callback(GLFWwindow* window, const float x_scale, const float y_scale)
{
}

static void key_callback(GLFWwindow* window, const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods)
{
    auto* data = static_cast<WindowUserData*>(glfwGetWindowUserPointer(window));

    if (!data)
    {
        return;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_M)
    {
        glfwMaximizeWindow(window);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        data->camera.ResetTarget();
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
    {
        data->draw.grid = !data->draw.grid;
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        data->draw.origin = !data->draw.origin;
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        data->draw.target = !data->draw.target;
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        data->camera_orthographic = !data->camera_orthographic;
    }

    if (key == GLFW_KEY_UP)
    {
        data->is_paused = true;
    }

    if (key == GLFW_KEY_DOWN)
    {
        data->is_paused = false;
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

static void cursor_position_callback(GLFWwindow* window, const double x_pos, const double y_pos)
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

    const auto f32_x_pos = static_cast<float>(x_pos);
    const auto f32_y_pos = static_cast<float>(y_pos);

    if (data->input.is_first_mouse)
    {
        data->input.is_first_mouse = false;

        data->input.last_mouse_position_x = f32_x_pos;
        data->input.last_mouse_position_y = f32_y_pos;
    }

    const auto delta_x = f32_x_pos - data->input.last_mouse_position_x;
    const auto delta_y = data->input.last_mouse_position_y - f32_y_pos;

    data->input.last_mouse_position_x = f32_x_pos;
    data->input.last_mouse_position_y = f32_y_pos;

    if (data->input.mouse_pressed_left)
    {
        data->camera.Orbit(delta_x, delta_y);
    }

    if (data->input.mouse_pressed_right)
    {
        data->camera.Pan(delta_x, delta_y);
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

static void scroll_callback(GLFWwindow* window, const double x_offset, const double y_offset)
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

    const auto f32_offset_y = static_cast<float>(y_offset);

    data->camera.Zoom(f32_offset_y);
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

static void monitor_callback(GLFWmonitor* monitor, const int32_t event)
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
    glfwSetMonitorCallback(monitor_callback);
}

int main()
{
    glfwSetErrorCallback(DebugCallbacks::DebugCallback_GLFW);

    if (!glfwInit())
    {
        spdlog::critical("Failed to initialize GLFW!");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "opengl-project-template-application", nullptr, nullptr);

    if (!window)
    {
        spdlog::critical("Failed to create GLFW window!");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    WindowUserData window_user_data           = {};
    window_user_data.app_time.last_time       = std::chrono::steady_clock::now();
    window_user_data.app_time.delta_time      = 0.0f;
    window_user_data.app_time.simulation_time = 0.0f;
    window_user_data.app_time.real_time       = 0.0f;
    window_user_data.input                    = {};
    window_user_data.camera                   = {};
    window_user_data.window_iconified         = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
    window_user_data.window_maximized         = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
    window_user_data.window_focused           = glfwGetWindowAttrib(window, GLFW_FOCUSED);
    window_user_data.window_cursor_entered    = glfwGetWindowAttrib(window, GLFW_HOVERED);
    glfwGetFramebufferSize(window, &window_user_data.framebuffer_width, &window_user_data.framebuffer_height);

    glfwSetWindowUserPointer(window, &window_user_data);

    register_callbacks(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize OpenGL GLAD!");
        return EXIT_FAILURE;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallbacks::DebugCallback_OpenGL, nullptr);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    SetImGUIStyle();

    constexpr std::array<ColorPoint3D, 6> origin_points = {
        ColorPoint3D{{0.0f, 0.0f, 0.0f}, {0xFF, 0x00, 0x00, 0xFF}},
        ColorPoint3D{{1.0f, 0.0f, 0.0f}, {0xFF, 0x00, 0x00, 0xFF}},
        ColorPoint3D{{0.0f, 0.0f, 0.0f}, {0x00, 0xFF, 0x00, 0xFF}},
        ColorPoint3D{{0.0f, 1.0f, 0.0f}, {0x00, 0xFF, 0x00, 0xFF}},
        ColorPoint3D{{0.0f, 0.0f, 0.0f}, {0x00, 0x00, 0xFF, 0xFF}},
        ColorPoint3D{{0.0f, 0.0f, 1.0f}, {0x00, 0x00, 0xFF, 0xFF}}};

    constexpr std::array<Point3D, 6> target_points = {
        Point3D{{-1.0f, 0.0f, 0.0f}},
        Point3D{{1.0f, 0.0f, 0.0f}},
        Point3D{{0.0f, -1.0f, 0.0f}},
        Point3D{{0.0f, 1.0f, 0.0f}},
        Point3D{{0.0f, 0.0f, -1.0f}},
        Point3D{{0.0f, 0.0f, 1.0f}}};

    constexpr std::array<Point2D, 4> plane_points = {
        Point2D{{-1.0f, -1.0f}},
        Point2D{{1.0f, -1.0f}},
        Point2D{{-1.0f, 1.0f}},
        Point2D{{1.0f, 1.0f}}};

    const ProgramDescriptor program_color_point_descriptor = {
        ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                         .source_size = static_cast<int32_t>(Default::ColorPointVertexSize()),
                         .source      = Default::ColorPointVertexCode()},
        ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                         .source_size = static_cast<int32_t>(Default::ColorPointFragmentSize()),
                         .source      = Default::ColorPointFragmentCode()}};

    const ProgramDescriptor program_point_descriptor = {
        ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                         .source_size = static_cast<int32_t>(Default::PointVertexSize()),
                         .source      = Default::PointVertexCode()},
        ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                         .source_size = static_cast<int32_t>(Default::PointFragmentSize()),
                         .source      = Default::PointFragmentCode()}};

    const ProgramDescriptor program_grid_descriptor = {
        ShaderDescriptor{.shader_type = GL_VERTEX_SHADER,
                         .source_size = static_cast<int32_t>(Default::GridVertexSize()),
                         .source      = Default::GridVertexCode()},
        ShaderDescriptor{.shader_type = GL_FRAGMENT_SHADER,
                         .source_size = static_cast<int32_t>(Default::GridFragmentSize()),
                         .source      = Default::GridFragmentCode()}};

    auto* buffer_system_transform_uniforms = new Buffer(GL_DYNAMIC_STORAGE_BIT, sizeof(SystemTransformUniforms), nullptr);

    auto* vertex_buffer_origin = new Buffer(GL_NONE, origin_points.size() * sizeof(ColorPoint3D), origin_points.data());
    auto* vertex_array_origin  = new VertexArray(vertex_buffer_origin, false, nullptr, false, Default::GetVertexLayout<ColorPoint3D>());

    auto* vertex_buffer_target = new Buffer(GL_NONE, target_points.size() * sizeof(Point3D), target_points.data());
    auto* vertex_array_target  = new VertexArray(vertex_buffer_target, false, nullptr, false, Default::GetVertexLayout<Point3D>());

    auto* vertex_buffer_grid = new Buffer(GL_NONE, plane_points.size() * sizeof(Point2D), plane_points.data());
    auto* vertex_array_grid  = new VertexArray(vertex_buffer_grid, false, nullptr, false, Default::GetVertexLayout<Point2D>());

    auto* program_color_point = new Program(program_color_point_descriptor);
    auto* program_point       = new Program(program_point_descriptor);
    auto* program_grid        = new Program(program_grid_descriptor);

    constexpr uint32_t gradient_width  = 256;
    constexpr uint32_t gradient_height = 256;

    glm::u8vec3 gradient_image_data[gradient_width * gradient_height] = {};
    ClearImageRGB8U(gradient_image_data, gradient_width, gradient_height);
    GenerateGradientImageRGB8U(gradient_image_data, gradient_width, gradient_height);

    constexpr TextureSpecification gradient_image_specification = {
        .width                  = gradient_width,
        .height                 = gradient_height,
        .opengl_internal_format = GL_RGB8,
        .opengl_format          = GL_RGB,
        .opengl_type            = GL_UNSIGNED_BYTE,
        .sampler_parameters     = {
                .opengl_texture_min_filter = GL_LINEAR,
                .opengl_texture_mag_filter = GL_LINEAR,
                .image_wrap_s              = GL_REPEAT,
                .image_wrap_t              = GL_REPEAT,
                .image_wrap_r              = GL_REPEAT}};

    auto* gradient_image = new Texture(gradient_image_specification);

    gradient_image->UpLoadExternalImage(gradient_image_data);

    auto* dynamic_renderer_point = new DynamicRenderer(128U * 1024U, 128U * 1024U);

    float        grid_spacing_major   = 1.0f;
    int32_t      grid_minor_per_major = 10;
    float        grid_fade_start      = 50.0f;
    float        grid_fade_end        = 75.0f;
    glm::f32vec4 grid_color_major     = {1.0f, 1.0f, 1.0f, 1.0f};
    glm::f32vec4 grid_color_minor     = {0.125f, 0.125f, 0.125f, 1.0f};

    float clear_color[] = {0.025f, 0.025f, 0.025f, 1.0f};

    glClearDepth(1.0f);
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        {
            const auto  now = std::chrono::steady_clock::now();
            const float dt  = std::chrono::duration<float>(now - window_user_data.app_time.last_time).count();

            window_user_data.app_time.last_time = now;
            window_user_data.app_time.real_time += dt;

            if (!window_user_data.is_paused)
            {
                window_user_data.app_time.simulation_time += dt;
                window_user_data.app_time.delta_time = dt;
            }
            else
            {
                window_user_data.app_time.delta_time = 0.0f;
            }
        }

        if (window_user_data.window_iconified)
        {
            glfwWaitEvents();

            window_user_data.app_time.last_time = std::chrono::steady_clock::now();

            continue;
        }

        glViewport(0, 0, window_user_data.framebuffer_width, window_user_data.framebuffer_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Camera& camera = window_user_data.camera;

        const float          distance = camera.GetDistance();
        const glm::f32mat4x4 view     = camera.GetViewMatrix();
        const glm::f32vec3&  position = camera.GetPosition();
        const glm::f32vec3&  target   = camera.GetTarget();
        const glm::f32vec3&  up       = camera.GetUp();
        const float          yaw      = camera.GetYaw();
        const float          pitch    = camera.GetPitch();
        const UpAxis         up_axis  = camera.GetUpAxis();

        const glm::f32mat4x4 projection = [&]()
        {
            const float aspect = static_cast<float>(window_user_data.framebuffer_width) / static_cast<float>(window_user_data.framebuffer_height);
            const float fov    = glm::radians(window_user_data.camera_fov_degrees);

            if (window_user_data.camera_orthographic)
            {
                const float height = 2.0f * distance * glm::tan(fov * 0.5f);
                const float width  = height * aspect;

                return glm::ortho(
                    -width * 0.5f,
                    width * 0.5f,
                    -height * 0.5f,
                    height * 0.5f,
                    window_user_data.camera_orthographic_plane_near,
                    window_user_data.camera_orthographic_plane_far);
            }

            return glm::perspective(
                fov,
                aspect,
                window_user_data.camera_perspective_plane_near,
                window_user_data.camera_perspective_plane_far);
        }();

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE))
        {
            const Ray ray =
                window_user_data.camera_orthographic ? GeneratePickRay_ORTHOGRAPHIC(
                                                           window_user_data.input.last_mouse_position_x,
                                                           window_user_data.input.last_mouse_position_y,
                                                           window_user_data.framebuffer_width,
                                                           window_user_data.framebuffer_height,
                                                           view,
                                                           projection)
                                                     : GeneratePickRay_PERSPECTIVE(
                                                           window_user_data.input.last_mouse_position_x,
                                                           window_user_data.input.last_mouse_position_y,
                                                           window_user_data.framebuffer_width,
                                                           window_user_data.framebuffer_height,
                                                           view,
                                                           projection,
                                                           position);

            const glm::f32vec3 plane_point  = glm::f32vec3(0.0f, 0.0f, 0.0f);
            const glm::f32vec3 plane_normal = UpAxisToGLM(up_axis);

            glm::f32vec3 hit_point = glm::f32vec3(0.0f, 0.0f, 0.0f);
            if (RayPlaneIntersection(ray, plane_point, plane_normal, hit_point))
            {
                spdlog::info("Hit: {:.3f}, {:.3f}, {:.3f}", hit_point.x, hit_point.y, hit_point.z);
            }
            else
            {
                spdlog::info("No intersection");
            }
        }

        SystemTransformUniforms system_transform_uniforms = {};
        system_transform_uniforms.preview_view_projection = projection * view;
        system_transform_uniforms.preview_projection      = projection;
        system_transform_uniforms.preview_view            = view;

        buffer_system_transform_uniforms->Upload(&system_transform_uniforms, sizeof(SystemTransformUniforms));
        buffer_system_transform_uniforms->SetAsShaderResource(GL_UNIFORM_BUFFER, 0, sizeof(SystemTransformUniforms), 0);

        program_grid->PushUniform1F32("uniform_GridSpacing", grid_spacing_major);
        program_grid->PushUniform1F32("uniform_GridMinorPerMajor", static_cast<float>(grid_minor_per_major));
        program_grid->PushUniform4F32("uniform_ColorMinor", grid_color_minor);
        program_grid->PushUniform4F32("uniform_ColorMajor", grid_color_major);
        program_grid->PushUniform1F32("uniform_FadeStart", grid_fade_start);
        program_grid->PushUniform1F32("uniform_FadeEnd", grid_fade_end);
        program_grid->PushUniformS32("uniform_CameraUpAxis", static_cast<int32_t>(up_axis));

        program_point->PushUniform16F32("uniform_Model", kIdentity);
        program_point->PushUniform4F32("uniform_Color", glm::one<glm::f32vec4>());
        program_color_point->PushUniform16F32("uniform_Model", kIdentity);

        if (window_user_data.draw.grid)
        {
            glDepthMask(GL_FALSE);
            program_grid->Bind();
            vertex_array_grid->Bind();
            vertex_array_grid->DrawArray(GL_TRIANGLE_STRIP, 4);
            vertex_array_grid->Unbind();
            program_grid->Unbind();
            glDepthMask(GL_TRUE);
        }

        if (window_user_data.draw.origin)
        {
            glLineWidth(static_cast<float>(window_user_data.draw.origin_line_width));
            program_color_point->PushUniform16F32("uniform_Model", glm::scale(kIdentity, glm::f32vec3(window_user_data.draw.origin_line_scale)));
            program_color_point->Bind();
            vertex_array_origin->Bind();
            vertex_array_origin->DrawArray(GL_LINES, 6);
            vertex_array_origin->Unbind();
            program_color_point->Unbind();
            glLineWidth(1.0f);
        }

        if (window_user_data.draw.target)
        {
            glLineWidth(static_cast<float>(window_user_data.draw.target_line_width));
            program_point->PushUniform16F32("uniform_Model", glm::translate(kIdentity, target) *
                                                                 glm::scale(kIdentity, glm::f32vec3(window_user_data.draw.target_line_scale)));
            program_point->Bind();
            vertex_array_target->Bind();
            vertex_array_target->DrawArray(GL_LINES, 6);
            vertex_array_target->Unbind();
            program_point->Unbind();
            glLineWidth(1.0f);
        }

        {
            std::vector<ColorPoint3D> points_helix{};
            std::vector<ColorPoint3D> points_spiral{};
            std::vector<ColorPoint3D> points_disk{};
            std::vector<ColorPoint3D> points_sphere{};
            std::vector<ColorPoint3D> gaussian_cube{};

            GenerateHelixColorPoints(points_helix, 0.5f, 4.0f, 32.0f, 32U * 1024U, UpAxis::DEFAULT, {0xFF, 0x00, 0x7F, 0xFF});
            GenerateSpiralColorPoints(points_spiral, 8.0f, 64.0f, 32U * 1024U, UpAxis::X, {0x00, 0xFF, 0xFF, 0xFF});
            GenerateDiscColorPoints(points_disk, 4.0f, 8U * 1024U, UpAxis::DEFAULT, {0x00, 0xFF, 0x7F, 0xFF});
            GenerateFibonacciSphereColorPoints(points_sphere, 4.0f, 8U * 1024U, {0xFF, 0x7F, 0xFF, 0xFF});
            GenerateRandomColorPointsUniformCube(gaussian_cube, {4.0f, 4.0f, 4.0f}, 32U * 1024U);

            DynamicRendererRenderPassInfo dynamic_renderer_render_pass_info  = {};
            dynamic_renderer_render_pass_info.is_orthographic                = window_user_data.camera_orthographic;
            dynamic_renderer_render_pass_info.camera                         = &camera;
            dynamic_renderer_render_pass_info.width                          = window_user_data.framebuffer_width;
            dynamic_renderer_render_pass_info.height                         = window_user_data.framebuffer_height;
            dynamic_renderer_render_pass_info.fov_degrees                    = window_user_data.camera_fov_degrees;
            dynamic_renderer_render_pass_info.camera_orthographic_plane_near = window_user_data.camera_orthographic_plane_near;
            dynamic_renderer_render_pass_info.camera_orthographic_plane_far  = window_user_data.camera_orthographic_plane_far;
            dynamic_renderer_render_pass_info.camera_perspective_plane_near  = window_user_data.camera_perspective_plane_near;
            dynamic_renderer_render_pass_info.camera_perspective_plane_far   = window_user_data.camera_perspective_plane_far;

            const float t = window_user_data.app_time.simulation_time;

            dynamic_renderer_point->RenderPassBegin(dynamic_renderer_render_pass_info);
            dynamic_renderer_point->SubmitColorPoint3D(GL_POINTS,
                                                       points_helix,
                                                       glm::rotate(kIdentity, t, glm::f32vec3(0.0f, 1.0f, 1.0f)) *
                                                           glm::translate(kIdentity, glm::f32vec3(-10.0f, 0.0f, -10.0f)) *
                                                           glm::rotate(kIdentity, t, glm::f32vec3(0.0f, 1.0f, 1.0f)) *
                                                           glm::scale(kIdentity, glm::f32vec3(1.0f, 1.0f, 1.0f) + 0.5f * glm::f32vec3(glm::sin(t), glm::cos(t), glm::sin(t))));
            dynamic_renderer_point->SubmitColorPoint3D(GL_LINE_STRIP,
                                                       points_spiral,
                                                       kIdentity);
            dynamic_renderer_point->SubmitColorPoint3D(GL_POINTS,
                                                       points_spiral,
                                                       glm::rotate(kIdentity, glm::radians(90.0f), glm::f32vec3(0.0f, 1.0f, 0.0f)));
            dynamic_renderer_point->SubmitColorPoint3D(GL_POINTS,
                                                       points_disk,
                                                       glm::rotate(kIdentity, t, glm::f32vec3(1.0f, 0.0f, 1.0f)) *
                                                           glm::translate(kIdentity, glm::f32vec3(0.0f, 10.0f, 0.0f)) *
                                                           glm::rotate(kIdentity, t, glm::f32vec3(1.0f, 0.0f, 1.0f)) *
                                                           glm::scale(kIdentity, glm::f32vec3(1.0f, 1.0f, 1.0f) + 0.5f * glm::f32vec3(glm::sin(t), glm::sin(t), glm::cos(t))));
            dynamic_renderer_point->SubmitColorPoint3D(GL_POINTS,
                                                       points_sphere,
                                                       glm::rotate(kIdentity, t, glm::f32vec3(1.0f, 1.0f, 0.0f)) *
                                                           glm::translate(kIdentity, glm::f32vec3(10.0f, 0.0f, 10.0f)) *
                                                           glm::rotate(kIdentity, t, glm::f32vec3(1.0f, 1.0f, 0.0f)) *
                                                           glm::scale(kIdentity, glm::f32vec3(1.0f, 1.0f, 1.0f) + 0.5f * glm::f32vec3(glm::sin(t), glm::cos(t), glm::cos(t))));
            dynamic_renderer_point->SubmitColorPoint3D(GL_POINTS,
                                                       gaussian_cube,
                                                       glm::rotate(kIdentity, t, glm::f32vec3(1.0f, 1.0f, 0.0f)) *
                                                           glm::translate(kIdentity, glm::f32vec3(0.0f, -1.0f, 1.0f)) *
                                                           glm::rotate(kIdentity, t, glm::f32vec3(1.0f, 1.0f, 0.0f)) *
                                                           glm::scale(kIdentity, glm::f32vec3(1.0f, 1.0f, 1.0f) + 0.5f * glm::f32vec3(glm::cos(t), glm::cos(t), glm::sin(t))));
            dynamic_renderer_point->RenderPassEnd();
        }

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();

        ImGui::NewFrame();

        if (ImGui::Begin("Settings"))
        {
            if (ImGui::TreeNode("Performance"))
            {
                ImGui::Text("System info sampling interval : %u ms", GetSampleIntervalNS() / (1000U * 1000U));
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("This interval ensures correct CPU and memory sampling. Updating too frequently may give inaccurate results.");
                }

                ImGui::Separator();

                if (ImGui::TreeNode("Timing"))
                {
                    ImGui::Text("Frame time : %.3f ms", ImGui::GetIO().DeltaTime * 1000.0f);
                    ImGui::Text("Frame rate : %.3f FPS", ImGui::GetIO().Framerate);

                    ImGui::TreePop();
                }

                ImGUINodes::DrawRAMInfoGUI();
                ImGUINodes::DrawProcessRAMInfoGUI();
                ImGUINodes::DrawGPUInfoGUI();
                ImGUINodes::DrawGPUMemoryInfoGUI();

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("WindowUserData"))
            {
                if (ImGui::TreeNode("time"))
                {
                    ImGui::Text("App time      : %.3f s", window_user_data.app_time.simulation_time);
                    ImGui::Text("Real time     : %.3f s", window_user_data.app_time.real_time);
                    ImGui::Text("Delta time    : %.5f s", window_user_data.app_time.delta_time);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("input"))
                {
                    ImGui::Text("last_mouse_position_x : %.2lf", window_user_data.input.last_mouse_position_x);
                    ImGui::Text("last_mouse_position_y : %.2lf", window_user_data.input.last_mouse_position_y);
                    ImGui::Text("is_first_mouse        : %s", window_user_data.input.is_first_mouse ? "Y" : "N");
                    ImGui::Text("mouse_pressed_left    : %s", window_user_data.input.mouse_pressed_left ? "Y" : "N");
                    ImGui::Text("mouse_pressed_right   : %s", window_user_data.input.mouse_pressed_right ? "Y" : "N");

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("camera"))
                {
                    ImGui::Text("position : [%.2f, %.2f, %.2f]", position.x, position.y, position.z);
                    ImGui::Text("target   : [%.2f, %.2f, %.2f]", target.x, target.y, target.z);
                    ImGui::Text("up       : [%.2f, %.2f, %.2f]", up.x, up.y, up.z);
                    ImGui::Text("yaw      : [%.2f]", yaw);
                    ImGui::Text("pitch    : [%.2f]", pitch);
                    ImGui::Text("distance : [%.2f]", distance);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("draw"))
                {
                    uint32_t aliased_line_range[2]  = {1, 1};
                    uint32_t aliased_point_range[2] = {1, 1};

                    {
                        float float_aliased_line_range[2]  = {0.0f, 0.0f};
                        float float_aliased_point_range[2] = {0.0f, 0.0f};

                        glGetFloatv(GL_LINE_WIDTH_RANGE, float_aliased_line_range);
                        glGetFloatv(GL_POINT_SIZE_RANGE, float_aliased_point_range);

                        aliased_line_range[0]  = static_cast<uint32_t>(float_aliased_line_range[0]);
                        aliased_line_range[1]  = static_cast<uint32_t>(float_aliased_line_range[1]);
                        aliased_point_range[0] = static_cast<uint32_t>(float_aliased_point_range[0]);
                        aliased_point_range[1] = static_cast<uint32_t>(float_aliased_point_range[1]);
                    }

                    ImGui::Checkbox("grid", &window_user_data.draw.grid);

                    ImGui::Separator();
                    ImGui::Checkbox("origin", &window_user_data.draw.origin);
                    if (window_user_data.draw.origin)
                    {
                        ImGui::DragFloat("origin_line_scale", &window_user_data.draw.origin_line_scale, 0.001f, 0.01f);
                        ImGui::DragScalar("origin_line_width", ImGuiDataType_U32, &window_user_data.draw.origin_line_width, 1.0f, &aliased_line_range[0], &aliased_line_range[1]);
                    }

                    ImGui::Separator();
                    ImGui::Checkbox("target", &window_user_data.draw.target);
                    if (window_user_data.draw.target)
                    {
                        ImGui::DragFloat("target_line_scale", &window_user_data.draw.target_line_scale, 0.001f, 0.01f);
                        ImGui::DragScalar("target_line_width", ImGuiDataType_U32, &window_user_data.draw.target_line_width, 1.0f, &aliased_line_range[0], &aliased_line_range[1]);
                    }

                    ImGui::TreePop();
                }

                ImGui::Text("window_iconified : %s", window_user_data.window_iconified ? "Y" : "N");
                ImGui::Text("window_maximized : %s", window_user_data.window_maximized ? "Y" : "N");
                ImGui::Text("window_focused : %s", window_user_data.window_focused ? "Y" : "N");
                ImGui::Text("window_cursor_entered : %s", window_user_data.window_cursor_entered ? "Y" : "N");
                ImGui::Text("framebuffer_width : %d", window_user_data.framebuffer_width);
                ImGui::Text("framebuffer_height : %d", window_user_data.framebuffer_height);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Camera"))
            {
                if (ImGui::TreeNode("Projection"))
                {
                    ImGui::Checkbox("camera_orthographic", &window_user_data.camera_orthographic);
                    ImGui::DragFloat("camera_fov_degrees", &window_user_data.camera_fov_degrees, 0.1f, 0.001f, 179.0f);
                    ImGui::DragFloat("camera_orthographic_plane_near", &window_user_data.camera_orthographic_plane_near, 0.001f, -1000.0f, 0.001f);
                    ImGui::DragFloat("camera_orthographic_plane_far", &window_user_data.camera_orthographic_plane_far, 0.001f, 0.001f, 1000.0f);
                    ImGui::DragFloat("camera_perspective_plane_near", &window_user_data.camera_perspective_plane_near, 0.001f, 0.001f, 1000.0f);
                    ImGui::DragFloat("camera_perspective_plane_far", &window_user_data.camera_perspective_plane_far, 0.001f, 0.001f, 1000.0f);
                    ImGui::TreePop();
                }

                if (ImGui::Button("Set camera UP X"))
                {
                    camera.SetUpAxis(UpAxis::X);
                }

                if (ImGui::Button("Set camera UP Y"))
                {
                    camera.SetUpAxis(UpAxis::Y);
                }

                if (ImGui::Button("Set camera UP Z"))
                {
                    camera.SetUpAxis(UpAxis::Z);
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Grid"))
            {
                constexpr float grid_spacing_major_speed = 0.5f;
                constexpr float grid_spacing_major_min   = 1.0f;
                constexpr float grid_spacing_major_max   = 10.0f;

                constexpr float   minor_per_major_speed = 1.0f;
                constexpr int32_t minor_per_major_min   = 2;
                constexpr int32_t minor_per_major_max   = 20;

                constexpr float fade_start_speed = 0.5f;
                constexpr float fade_start_min   = 1.0f;
                constexpr float fade_start_max   = 100.0f;

                constexpr float fade_end_speed = 0.5f;
                constexpr float fade_end_min   = 1.0f;
                constexpr float fade_end_max   = fade_start_max + 1.0f;

                ImGui::DragScalar("grid_spacing_major", ImGuiDataType_Float, &grid_spacing_major, grid_spacing_major_speed, &grid_spacing_major_min, &grid_spacing_major_max);
                ImGui::DragScalar("minor_per_major", ImGuiDataType_S32, &grid_minor_per_major, minor_per_major_speed, &minor_per_major_min, &minor_per_major_max);
                ImGui::DragScalar("fade_start", ImGuiDataType_Float, &grid_fade_start, fade_start_speed, &fade_start_min, &fade_start_max);
                ImGui::DragScalar("fade_end", ImGuiDataType_Float, &grid_fade_end, fade_end_speed, &fade_end_min, &fade_end_max);

                if (grid_fade_end <= grid_fade_start)
                {
                    grid_fade_end = grid_fade_start + 1;
                }

                ImGui::ColorEdit3("grid_color_major", glm::value_ptr(grid_color_major));
                ImGui::ColorEdit3("grid_color_minor", glm::value_ptr(grid_color_minor));

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Clear color"))
            {
                if (ImGui::ColorEdit3("clear_color", clear_color))
                {
                    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Gradient image"))
            {
                constexpr float display_image_size_multiplier = 4.0f;

                const float display_image_width  = display_image_size_multiplier * static_cast<float>(gradient_image->GetTextureSpecification().width);
                const float display_image_height = display_image_size_multiplier * static_cast<float>(gradient_image->GetTextureSpecification().height);

                constexpr float display_image_uv0_x = -1.0f;
                constexpr float display_image_uv0_y = -3.0f;

                constexpr float display_image_uv1_x = 3.0f;
                constexpr float display_image_uv1_y = 2.0f;

                ImGui::Image((ImTextureRef)gradient_image->GetID(),
                             {display_image_width, display_image_height},
                             {display_image_uv0_x, display_image_uv0_y},
                             {display_image_uv1_x, display_image_uv1_y});

                ImGui::TreePop();
            }
        }
        ImGui::End();

        ImGui::EndFrame();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    delete dynamic_renderer_point;

    delete gradient_image;
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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
