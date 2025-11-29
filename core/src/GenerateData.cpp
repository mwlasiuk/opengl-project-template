#include <Core/Utils/GenerateData.hpp>

#include <array>
#include <cstring>
#include <random>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

namespace Core
{
    static constexpr float kGoldenAngle = 2.399963229728653f;

    namespace Common
    {
        [[nodiscard]] glm::f32vec3 Disc_PointFromUpAxis(const UpAxis up_axis, const float theta, const float rr)
        {
            glm::f32vec3 point{};

            const float theta_sin = glm::sin(theta);
            const float theta_cos = glm::cos(theta);

            switch (up_axis)
            {
            case UpAxis::X: // YZ plane
                {
                    point = {0.0f, theta_cos, theta_sin};
                    break;
                }
            case UpAxis::Y: // XZ plane
                {
                    point = {theta_cos, 0.0f, theta_sin};
                    break;
                }
            case UpAxis::Z: // XY plane
                {
                    point = {theta_cos, theta_sin, 0.0f};
                    break;
                }
            }

            return point * rr;
        }

        [[nodiscard]] std::array<glm::f32vec3, 4> Quad_PointFromUpAxis(const UpAxis up_axis, const float hx, const float hy)
        {
            std::array<glm::f32vec3, 4> points{};

            switch (up_axis)
            {
            case UpAxis::X:
                {
                    points[0] = {0.0f, -hx, -hy};
                    points[1] = {0.0f, hx, -hy};
                    points[2] = {0.0f, hx, hy};
                    points[3] = {0.0f, -hx, hy};
                    break;
                }
            case UpAxis::Y:
                {
                    points[0] = {-hx, 0.0f, -hy};
                    points[1] = {hx, 0.0f, -hy};
                    points[2] = {hx, 0.0f, hy};
                    points[3] = {-hx, 0.0f, hy};
                    break;
                }
            case UpAxis::Z:
                {
                    points[0] = {-hx, -hy, 0.0f};
                    points[1] = {hx, -hy, 0.0f};
                    points[2] = {hx, hy, 0.0f};
                    points[3] = {-hx, hy, 0.0f};
                    break;
                }
            }

            return points;
        }

        [[nodiscard]] glm::f32vec3 Circle_PointFromUpAxis(const UpAxis up_axis, const float a, const float r)
        {
            glm::f32vec3 point{};

            const float a_sin = glm::sin(a);
            const float a_cos = glm::cos(a);

            switch (up_axis)
            {
            case UpAxis::X:
                {
                    point = {0, a_cos, a_sin};
                    break;
                }
            case UpAxis::Y:
                {
                    point = {a_cos, 0, a_sin};
                    break;
                }
            case UpAxis::Z:
                {
                    point = {a_cos, a_sin, 0};
                    break;
                }
            }

            return point * r;
        }

        [[nodiscard]] glm::f32vec3 Spiral_PointFromUpAxis(const UpAxis up_axis, const float a, const float r)
        {
            glm::f32vec3 point{};

            const float a_sin = glm::sin(a);
            const float a_cos = glm::cos(a);

            switch (up_axis)
            {
            case UpAxis::X:
                {
                    point = {0.0f, a_cos, a_sin};
                    break;
                }
            case UpAxis::Y:
                {
                    point = {a_cos, 0.0f, a_sin};
                    break;
                }
            case UpAxis::Z:
                {
                    point = {a_cos, a_sin, 0.0f};
                    break;
                }
            }

            return point * r;
        }

        [[nodiscard]] glm::f32vec3 Helix_PointFromUpAxis(const UpAxis up_axis, const float t, const float a, const float r, const float h)
        {
            glm::f32vec3 point{};
            glm::f32vec3 scale{};

            const float a_sin = glm::sin(a);
            const float a_cos = glm::cos(a);

            switch (up_axis)
            {
            case UpAxis::X:
                {
                    point = {h, a_cos, a_sin};
                    scale = {t, r, r};
                    break;
                }
            case UpAxis::Y:
                {
                    point = {a_cos, h, a_sin};
                    scale = {r, t, r};
                    break;
                }
            case UpAxis::Z:
                {
                    point = {a_cos, a_sin, h};
                    scale = {r, r, t};
                    break;
                }
            }

            return point * scale;
        }
    }

    static std::mt19937 CreateSeededRNG()
    {
        std::random_device rd{};
        std::seed_seq      seq{rd(), rd(), rd(), rd()};

        return std::mt19937(seq);
    }

    static std::mt19937& CreateRNG()
    {
        thread_local std::mt19937 rng = CreateSeededRNG();
        return rng;
    }

    void GenerateRandomPointsUniformCube(std::vector<Point3D>& points, const glm::f32vec3& extent, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution distribution_x(-extent.x / 2.0f, extent.x / 2.0f);
        std::uniform_real_distribution distribution_y(-extent.y / 2.0f, extent.y / 2.0f);
        std::uniform_real_distribution distribution_z(-extent.z / 2.0f, extent.z / 2.0f);

        for (Point3D& point : points)
        {
            point.position.x = distribution_x(rng);
            point.position.y = distribution_y(rng);
            point.position.z = distribution_z(rng);
        }
    }

    void GenerateRandomColorPointsUniformCube(std::vector<ColorPoint3D>& points, const glm::f32vec3& extent, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution distribution_x(-extent.x / 2.0f, extent.x / 2.0f);
        std::uniform_real_distribution distribution_y(-extent.y / 2.0f, extent.y / 2.0f);
        std::uniform_real_distribution distribution_z(-extent.z / 2.0f, extent.z / 2.0f);

        std::uniform_int_distribution<uint32_t> dist_color(0x000000, 0xFFFFFF);

        for (ColorPoint3D& point : points)
        {
            point.position.x = distribution_x(rng);
            point.position.y = distribution_y(rng);
            point.position.z = distribution_z(rng);

            const uint32_t rgb = dist_color(rng);

            point.color.x = static_cast<uint8_t>((rgb >> 8 * 2) & 0xFF); // R
            point.color.y = static_cast<uint8_t>((rgb >> 8 * 1) & 0xFF); // G
            point.color.z = static_cast<uint8_t>((rgb >> 8 * 0) & 0xFF); // B
            point.color.w = 0xFF;
        }
    }

    void GenerateRandomPointsGaussianCube(std::vector<Point3D>& points, const glm::f32vec3& extent, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::normal_distribution<float> dist_x(0.0f, extent.x / 6.0f);
        std::normal_distribution<float> dist_y(0.0f, extent.y / 6.0f);
        std::normal_distribution<float> dist_z(0.0f, extent.z / 6.0f);

        for (auto& point : points)
        {
            point.position.x = dist_x(rng);
            point.position.y = dist_y(rng);
            point.position.z = dist_z(rng);
        }
    }

    void GenerateRandomColorPointsGaussianCube(std::vector<ColorPoint3D>& points, const glm::f32vec3& extent, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::normal_distribution<float> dist_x(0.0f, extent.x / 6.0f);
        std::normal_distribution<float> dist_y(0.0f, extent.y / 6.0f);
        std::normal_distribution<float> dist_z(0.0f, extent.z / 6.0f);

        std::uniform_int_distribution<uint32_t> dist_color(0x000000, 0xFFFFFF);

        for (auto& point : points)
        {
            point.position.x = dist_x(rng);
            point.position.y = dist_y(rng);
            point.position.z = dist_z(rng);

            const uint32_t rgb = dist_color(rng);

            point.color.x = static_cast<uint8_t>((rgb >> 8 * 2) & 0xFF); // R
            point.color.y = static_cast<uint8_t>((rgb >> 8 * 1) & 0xFF); // G
            point.color.z = static_cast<uint8_t>((rgb >> 8 * 0) & 0xFF); // B
            point.color.w = 0xFF;
        }
    }

    void GenerateRandomPointsUniformSphere(std::vector<Point3D>& points, const float radius, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution<float> dist_u(-1.0f, 1.0f);
        std::uniform_real_distribution<float> dist_theta(0.0f, glm::two_pi<float>());
        std::uniform_real_distribution<float> dist_r(0.0f, 1.0f);

        for (auto& point : points)
        {
            float u     = dist_u(rng);
            float theta = dist_theta(rng);
            float r     = std::cbrt(dist_r(rng)) * radius;

            float s = std::sqrt(1.0f - u * u);

            point.position.x = r * s * std::cos(theta);
            point.position.y = r * u;
            point.position.z = r * s * std::sin(theta);
        }
    }

    void GenerateRandomColorPointsUniformSphere(std::vector<ColorPoint3D>& points, const float radius, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution<float> dist_u(-1.0f, 1.0f);
        std::uniform_real_distribution<float> dist_theta(0.0f, glm::two_pi<float>());
        std::uniform_real_distribution<float> dist_r(0.0f, 1.0f);

        std::uniform_int_distribution<uint32_t> dist_color(0x000000, 0xFFFFFF);

        for (auto& point : points)
        {
            float u     = dist_u(rng);
            float theta = dist_theta(rng);
            float r     = std::cbrt(dist_r(rng)) * radius;

            float s = std::sqrt(1.0f - u * u);

            point.position.x = r * s * std::cos(theta);
            point.position.y = r * u;
            point.position.z = r * s * std::sin(theta);

            const uint32_t rgb = dist_color(rng);

            point.color.x = static_cast<uint8_t>((rgb >> 8 * 2) & 0xFF); // R
            point.color.y = static_cast<uint8_t>((rgb >> 8 * 1) & 0xFF); // G
            point.color.z = static_cast<uint8_t>((rgb >> 8 * 0) & 0xFF); // B
            point.color.w = 0xFF;
        }
    }

    void GenerateRandomPointsGaussianSphere(std::vector<Point3D>& points, const float radius, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::normal_distribution<float> dist(0.0f, radius / 6.0f);

        for (auto& point : points)
        {
            point.position.x = dist(rng);
            point.position.y = dist(rng);
            point.position.z = dist(rng);
        }
    }

    void GenerateRandomColorPointsGaussianSphere(std::vector<ColorPoint3D>& points, const float radius, const size_t count)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::normal_distribution<float>         dist(0.0f, radius / 6.0f);
        std::uniform_int_distribution<uint32_t> dist_color(0x000000, 0xFFFFFF);

        for (auto& point : points)
        {
            point.position.x = dist(rng);
            point.position.y = dist(rng);
            point.position.z = dist(rng);

            const uint32_t rgb = dist_color(rng);

            point.color.x = static_cast<uint8_t>((rgb >> 8 * 2) & 0xFF); // R
            point.color.y = static_cast<uint8_t>((rgb >> 8 * 1) & 0xFF); // G
            point.color.z = static_cast<uint8_t>((rgb >> 8 * 0) & 0xFF); // B
            point.color.w = 0xFF;
        }
    }

    void GenerateCubePointsIndexed(std::vector<Point3D>& points, std::vector<uint32_t>& indices, const glm::f32vec3& extent)
    {
        const glm::f32vec3 half_extent = extent * 0.5f;

        points = {
            Point3D{{-half_extent.x, -half_extent.y, -half_extent.z}},
            Point3D{{half_extent.x, -half_extent.y, -half_extent.z}},
            Point3D{{half_extent.x, half_extent.y, -half_extent.z}},
            Point3D{{-half_extent.x, half_extent.y, -half_extent.z}},
            Point3D{{-half_extent.x, -half_extent.y, half_extent.z}},
            Point3D{{half_extent.x, -half_extent.y, half_extent.z}},
            Point3D{{half_extent.x, half_extent.y, half_extent.z}},
            Point3D{{-half_extent.x, half_extent.y, half_extent.z}}};

        indices = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
    }

    void GenerateCubeColorPointsIndexed(std::vector<ColorPoint3D>& points, std::vector<uint32_t>& indices, const glm::f32vec3& extent, const glm::u8vec4& color)
    {
        const glm::f32vec3 half_extent = extent * 0.5f;

        points = {
            ColorPoint3D{{-half_extent.x, -half_extent.y, -half_extent.z}, color},
            ColorPoint3D{{half_extent.x, -half_extent.y, -half_extent.z}, color},
            ColorPoint3D{{half_extent.x, half_extent.y, -half_extent.z}, color},
            ColorPoint3D{{-half_extent.x, half_extent.y, -half_extent.z}, color},
            ColorPoint3D{{-half_extent.x, -half_extent.y, half_extent.z}, color},
            ColorPoint3D{{half_extent.x, -half_extent.y, half_extent.z}, color},
            ColorPoint3D{{half_extent.x, half_extent.y, half_extent.z}, color},
            ColorPoint3D{{-half_extent.x, half_extent.y, half_extent.z}, color}};

        indices = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};
    }

    void GenerateSpherePointsIndexed(std::vector<Point3D>& points, std::vector<uint32_t>& indices, const float radius, const uint32_t segments, const uint32_t rings)
    {
        points.clear();
        indices.clear();

        points.reserve((segments + 1) * (rings + 1));
        indices.reserve(rings * segments * 4 * 2); // rough line estimate

        for (uint32_t y = 0; y <= rings; y++)
        {
            float v   = (float)y / rings;
            float phi = v * glm::pi<float>();

            for (uint32_t x = 0; x <= segments; x++)
            {
                float u     = (float)x / segments;
                float theta = u * glm::two_pi<float>();

                glm::f32vec3 n{};
                n.x = std::sin(phi) * std::cos(theta);
                n.y = std::cos(phi);
                n.z = std::sin(phi) * std::sin(theta);

                points.push_back(Point3D{n * radius});
            }
        }

        for (uint32_t y = 0; y <= rings; y++)
        {
            for (uint32_t x = 0; x < segments; x++)
            {
                uint32_t i0 = y * (segments + 1) + x;
                uint32_t i1 = i0 + 1;

                indices.push_back(i0);
                indices.push_back(i1);
            }
        }

        for (uint32_t x = 0; x <= segments; x++)
        {
            for (uint32_t y = 0; y < rings; y++)
            {
                uint32_t i0 = y * (segments + 1) + x;
                uint32_t i1 = i0 + (segments + 1);

                indices.push_back(i0);
                indices.push_back(i1);
            }
        }
    }

    void GenerateSphereColorPointsIndexed(std::vector<ColorPoint3D>& points, std::vector<uint32_t>& indices, const float radius, const uint32_t segments, const uint32_t rings, const glm::u8vec4& color)
    {
        points.clear();
        indices.clear();

        points.reserve((segments + 1) * (rings + 1));
        indices.reserve(rings * segments * 4 * 2); // rough line estimate

        for (uint32_t y = 0; y <= rings; y++)
        {
            float v   = (float)y / rings;
            float phi = v * glm::pi<float>();

            for (uint32_t x = 0; x <= segments; x++)
            {
                float u     = (float)x / segments;
                float theta = u * glm::two_pi<float>();

                glm::f32vec3 n{};
                n.x = std::sin(phi) * std::cos(theta);
                n.y = std::cos(phi);
                n.z = std::sin(phi) * std::sin(theta);

                points.push_back(ColorPoint3D{n * radius, color});
            }
        }

        for (uint32_t y = 0; y <= rings; y++)
        {
            for (uint32_t x = 0; x < segments; x++)
            {
                uint32_t i0 = y * (segments + 1) + x;
                uint32_t i1 = i0 + 1;

                indices.push_back(i0);
                indices.push_back(i1);
            }
        }

        for (uint32_t x = 0; x <= segments; x++)
        {
            for (uint32_t y = 0; y < rings; y++)
            {
                uint32_t i0 = y * (segments + 1) + x;
                uint32_t i1 = i0 + (segments + 1);

                indices.push_back(i0);
                indices.push_back(i1);
            }
        }
    }

    void GenerateFibonacciSpherePoints(std::vector<Point3D>& points, const float radius, const size_t count)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t     = (float)i / (float)count;
            const float y     = 1.0f - 2.0f * t;
            const float r     = std::sqrt(1.0f - y * y);
            const float theta = kGoldenAngle * i;

            points[i].position.x = std::cos(theta) * r * radius;
            points[i].position.y = y * radius;
            points[i].position.z = std::sin(theta) * r * radius;
        }
    }

    void GenerateFibonacciSphereColorPoints(std::vector<ColorPoint3D>& points, const float radius, const size_t count, const glm::u8vec4& color)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t     = (float)i / (float)count;
            const float y     = 1.0f - 2.0f * t;
            const float r     = std::sqrt(1.0f - y * y);
            const float theta = kGoldenAngle * i;

            points[i].position.x = std::cos(theta) * r * radius;
            points[i].position.y = y * radius;
            points[i].position.z = std::sin(theta) * r * radius;

            points[i].color = color;
        }
    }

    void GenerateDiscPoints(std::vector<Point3D>& points, const float radius, const size_t count, const UpAxis up_axis)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution<float> angle(0.0f, glm::two_pi<float>());
        std::uniform_real_distribution<float> r(0.0f, 1.0f);

        for (auto& p : points)
        {
            const float theta = angle(rng);
            const float rr    = radius * std::sqrt(r(rng));

            p.position = Common::Disc_PointFromUpAxis(up_axis, theta, rr);
        }
    }

    void GenerateDiscColorPoints(std::vector<ColorPoint3D>& points, const float radius, const size_t count, const UpAxis up_axis, const glm::u8vec4& color)
    {
        points.resize(count);

        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution<float> angle(0.0f, glm::two_pi<float>());
        std::uniform_real_distribution<float> r(0.0f, 1.0f);

        for (auto& p : points)
        {
            const float theta = angle(rng);
            const float rr    = radius * std::sqrt(r(rng));

            p.position = Common::Disc_PointFromUpAxis(up_axis, theta, rr);
            p.color    = color;
        }
    }

    void GenerateQuadPoints(std::vector<Point3D>& points, const glm::f32vec2& extent, const UpAxis up_axis)
    {
        points.resize(4);

        const float hx = extent.x / 2.0f;
        const float hy = extent.y / 2.0f;

        const auto positions = Common::Quad_PointFromUpAxis(up_axis, hx, hy);

        points[0].position = positions[0];
        points[1].position = positions[1];
        points[2].position = positions[2];
        points[3].position = positions[3];
    }

    void GenerateQuadColorPoints(std::vector<ColorPoint3D>& points, const glm::f32vec2& extent, const UpAxis up_axis, const glm::u8vec4& color)
    {
        points.resize(4);

        const float hx = extent.x / 2.0f;
        const float hy = extent.y / 2.0f;

        const auto positions = Common::Quad_PointFromUpAxis(up_axis, hx, hy);

        points[0].position = positions[0];
        points[1].position = positions[1];
        points[2].position = positions[2];
        points[3].position = positions[3];

        for (ColorPoint3D& p : points)
        {
            p.color = color;
        }
    }

    void GenerateQuadPointsTriangles(std::vector<Point3D>& points, const glm::f32vec2& extent, const UpAxis up_axis)
    {
        points.resize(6);

        std::vector<Point3D> quad;
        GenerateQuadPoints(quad, extent, up_axis);

        points[0] = quad[0];
        points[1] = quad[1];
        points[2] = quad[2];

        points[3] = quad[0];
        points[4] = quad[2];
        points[5] = quad[3];
    }

    void GenerateQuadColorPointsTriangles(std::vector<ColorPoint3D>& points, const glm::f32vec2& extent, const UpAxis up_axis, const glm::u8vec4& color)
    {
        points.resize(6);

        std::vector<ColorPoint3D> quad;
        GenerateQuadColorPoints(quad, extent, up_axis);

        points[0] = quad[0];
        points[1] = quad[1];
        points[2] = quad[2];

        points[3] = quad[0];
        points[4] = quad[2];
        points[5] = quad[3];

        for (auto& p : points)
        {
            p.color = color;
        }
    }

    void GenerateGradientImageR8U(uint8_t* const image, const uint32_t width, const uint32_t height)
    {
        const uint32_t size = width * height;

        for (uint32_t i = 0; i < size; ++i)
        {
            const uint32_t x = i % width;

            image[i] = static_cast<uint8_t>(x * 0xFF / (width - 1));
        }
    }

    void GenerateGradientImageRG8U(glm::u8vec2* const image, const uint32_t width, const uint32_t height)
    {
        const uint32_t size = width * height;

        for (uint32_t i = 0; i < size; ++i)
        {
            const uint32_t x = i % width;
            const uint32_t y = i / width;

            image[i].r = static_cast<uint8_t>(x * 0xFF / (width - 1));
            image[i].g = static_cast<uint8_t>(y * 0xFF / (height - 1));
        }
    }

    void GenerateGradientImageRGB8U(glm::u8vec3* const image, const uint32_t width, const uint32_t height)
    {
        const uint32_t size = width * height;

        for (uint32_t i = 0; i < size; ++i)
        {
            const uint32_t x = i % width;
            const uint32_t y = i / width;

            image[i].r = static_cast<uint8_t>(x * 0xFF / (width - 1));
            image[i].g = static_cast<uint8_t>(y * 0xFF / (height - 1));
            image[i].b = static_cast<uint8_t>((x + y) * 0xFF / (width + height - 2));
        }
    }

    void GenerateGradientImageRGBA8U(glm::u8vec4* const image, const uint32_t width, const uint32_t height)
    {
        const uint32_t size = width * height;

        for (uint32_t i = 0; i < size; ++i)
        {
            const uint32_t x = i % width;
            const uint32_t y = i / width;

            image[i].r = static_cast<uint8_t>(x * 0xFF / (width - 1));
            image[i].g = static_cast<uint8_t>(y * 0xFF / (height - 1));
            image[i].b = static_cast<uint8_t>((x + y) * 0xFF / (width + height - 2));
            image[i].a = 0xFF;
        }
    }

    void ClearImageR8U(uint8_t* const image, const uint32_t width, const uint32_t height, const uint8_t value)
    {
        std::memset(image, value, width * height * sizeof(uint8_t));
    }

    void ClearImageRG8U(glm::u8vec2* const image, const uint32_t width, const uint32_t height, const glm::u8vec2& value)
    {
        const uint32_t size = width * height;

        if (value.r == value.g)
        {
            std::memset(image, value.r, size * sizeof(glm::u8vec2));
        }
        else
        {
            for (uint32_t i = 0; i < size; ++i)
            {
                image[i] = value;
            }
        }
    }

    void ClearImageRGB8U(glm::u8vec3* const image, const uint32_t width, const uint32_t height, const glm::u8vec3& value)
    {
        const uint32_t size = width * height;

        if (value.r == value.g && value.g == value.b)
        {
            std::memset(image, value.r, size * sizeof(glm::u8vec3));
        }
        else
        {
            for (uint32_t i = 0; i < size; ++i)
            {
                image[i] = value;
            }
        }
    }

    void ClearImageRGBA8U(glm::u8vec4* const image, const uint32_t width, const uint32_t height, const glm::u8vec4& value)
    {
        const uint32_t size = width * height;

        if (value.r == value.g && value.g == value.b && value.b == value.a)
        {
            std::memset(image, value.r, size * sizeof(glm::u8vec4));
        }
        else
        {
            for (uint32_t i = 0; i < size; ++i)
            {
                image[i] = value;
            }
        }
    }

    void ApplyPointJitter(std::vector<Point3D>& points, const float amount)
    {
        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        for (auto& p : points)
        {
            p.position.x += dist(rng) * amount;
            p.position.y += dist(rng) * amount;
            p.position.z += dist(rng) * amount;
        }
    }

    void ApplyColorPointJitter(std::vector<ColorPoint3D>& points, const float amount)
    {
        std::mt19937& rng = CreateRNG();

        std::uniform_real_distribution<float>   dist(-1.0f, 1.0f);
        std::uniform_int_distribution<uint32_t> dist_color(0x000000, 0xFFFFFF);

        for (auto& p : points)
        {
            p.position.x += dist(rng) * amount;
            p.position.y += dist(rng) * amount;
            p.position.z += dist(rng) * amount;
        }
    }

    void ApplyPointWarp(std::vector<Point3D>& points, void (*const fn)(const Point3D&))
    {
        for (auto& p : points)
        {
            fn(p);
        }
    }

    void ApplyColorPointWarp(std::vector<ColorPoint3D>& points, void (*const fn)(ColorPoint3D&))
    {
        for (auto& p : points)
        {
            fn(p);
        }
    }

    void NormalizePointCloud(std::vector<Point3D>& points, const glm::vec3& extent)
    {
        if (points.empty())
        {
            return;
        }

        glm::vec3 minV(std::numeric_limits<float>::max());
        glm::vec3 maxV(std::numeric_limits<float>::lowest());

        for (const auto& p : points)
        {
            minV = glm::min(minV, p.position);
            maxV = glm::max(maxV, p.position);
        }

        const glm::vec3 center = (minV + maxV) * 0.5f;
        const glm::vec3 size   = maxV - minV;

        glm::vec3 scale(1.0f);
        scale.x = (size.x > 0.0f) ? (extent.x / size.x) : 1.0f;
        scale.y = (size.y > 0.0f) ? (extent.y / size.y) : 1.0f;
        scale.z = (size.z > 0.0f) ? (extent.z / size.z) : 1.0f;

        for (auto& p : points)
        {
            p.position = (p.position - center) * scale;
        }
    }

    void NormalizeColorPointCloud(std::vector<ColorPoint3D>& points, const glm::vec3& extent)
    {
        if (points.empty())
        {
            return;
        }

        glm::vec3 minV(std::numeric_limits<float>::max());
        glm::vec3 maxV(std::numeric_limits<float>::lowest());

        for (const auto& p : points)
        {
            minV = glm::min(minV, p.position);
            maxV = glm::max(maxV, p.position);
        }

        const glm::vec3 center = (minV + maxV) * 0.5f;
        const glm::vec3 size   = maxV - minV;

        glm::vec3 scale(1.0f);
        scale.x = (size.x > 0.0f) ? (extent.x / size.x) : 1.0f;
        scale.y = (size.y > 0.0f) ? (extent.y / size.y) : 1.0f;
        scale.z = (size.z > 0.0f) ? (extent.z / size.z) : 1.0f;

        for (auto& p : points)
        {
            p.position = (p.position - center) * scale;
        }
    }

    void CenterAndScalePointCloud(std::vector<Point3D>& points, const float scale)
    {
        if (points.empty())
        {
            return;
        }

        glm::vec3 center(0.0f);

        for (const auto& p : points)
        {
            center += p.position;
        }

        center /= static_cast<float>(points.size());

        for (auto& p : points)
        {
            p.position = (p.position - center) * scale;
        }
    }

    void CenterAndScaleColorPointCloud(std::vector<ColorPoint3D>& points, const float scale)
    {
        if (points.empty())
        {
            return;
        }

        glm::vec3 center(0.0f);

        for (const auto& p : points)
        {
            center += p.position;
        }

        center /= static_cast<float>(points.size());

        for (auto& p : points)
        {
            p.position = (p.position - center) * scale;
        }
    }

    void GenerateLinePoints(std::vector<Point3D>& points, const glm::vec3& a, const glm::vec3& b, const size_t count)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t      = (count > 1) ? (float)i / (float)(count - 1) : 0.0f;
            points[i].position = glm::mix(a, b, t);
        }
    }

    void GenerateLineColorPoints(std::vector<ColorPoint3D>& points, const glm::vec3& a, const glm::vec3& b, const size_t count, const glm::u8vec4& color)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t      = (count > 1) ? (float)i / (float)(count - 1) : 0.0f;
            points[i].position = glm::mix(a, b, t);
            points[i].color    = color;
        }
    }

    void GeneratePolylinePoints(std::vector<Point3D>& points, const std::vector<glm::vec3>& control_points, const size_t points_per_segment)
    {
        points.clear();

        if (control_points.size() < 2)
        {
            return;
        }

        for (size_t i = 0; i < control_points.size() - 1; i++)
        {
            const auto& a = control_points[i];
            const auto& b = control_points[i + 1];

            for (size_t j = 0; j < points_per_segment; j++)
            {
                const float t = (float)j / (float)points_per_segment;
                points.push_back({glm::mix(a, b, t)});
            }
        }
    }

    void GeneratePolylineColorPoints(std::vector<ColorPoint3D>& points, const std::vector<glm::vec3>& control_points, const size_t points_per_segment, const glm::u8vec4& color)
    {
        points.clear();

        if (control_points.size() < 2)
        {
            return;
        }

        for (size_t i = 0; i < control_points.size() - 1; i++)
        {
            const auto& a = control_points[i];
            const auto& b = control_points[i + 1];

            for (size_t j = 0; j < points_per_segment; j++)
            {
                const float t = (float)j / (float)points_per_segment;
                points.push_back(ColorPoint3D{glm::mix(a, b, t), color});
            }
        }
    }

    void GenerateCirclePoints(std::vector<Point3D>& points, const float radius, const size_t segments, const UpAxis up_axis)
    {
        points.resize(segments);

        const float step = glm::two_pi<float>() / (float)segments;

        for (size_t i = 0; i < segments; i++)
        {
            const float a = step * (float)i;

            points[i].position = Common::Circle_PointFromUpAxis(up_axis, a, radius);
        }
    }

    void GenerateCircleColorPoints(std::vector<ColorPoint3D>& points, const float radius, const size_t segments, const UpAxis up_axis, const glm::u8vec4& color)
    {
        points.resize(segments);

        const float step = glm::two_pi<float>() / (float)segments;

        for (size_t i = 0; i < segments; i++)
        {
            const float a = step * (float)i;

            points[i].position = Common::Circle_PointFromUpAxis(up_axis, a, radius);
            points[i].color    = color;
        }
    }

    void GenerateSpiralPoints(std::vector<Point3D>& points, const float radius, const float turns, const size_t count, const UpAxis up_axis)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t = (float)i / (float)(count - 1);
            const float a = t * turns * glm::two_pi<float>();
            const float r = t * radius;

            points[i].position = Common::Spiral_PointFromUpAxis(up_axis, a, r);
        }
    }

    void GenerateSpiralColorPoints(std::vector<ColorPoint3D>& points, const float radius, const float turns, const size_t count, const UpAxis up_axis, const glm::u8vec4& color)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t = (float)i / (float)(count - 1);
            const float a = t * turns * glm::two_pi<float>();
            const float r = t * radius;

            points[i].position = Common::Spiral_PointFromUpAxis(up_axis, a, r);
            points[i].color    = color;
        }
    }

    void GenerateHelixPoints(std::vector<Point3D>& points, const float radius, const float height, const float turns, const size_t count, const UpAxis up_axis)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t = (float)i / (float)(count - 1);
            const float a = t * turns * glm::two_pi<float>();

            points[i].position = Common::Helix_PointFromUpAxis(up_axis, t, a, radius, height);
        }
    }

    void GenerateHelixColorPoints(std::vector<ColorPoint3D>& points, const float radius, const float height, const float turns, const size_t count, const UpAxis up_axis, const glm::u8vec4& color)
    {
        points.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            const float t = (float)i / (float)(count - 1);
            const float a = t * turns * glm::two_pi<float>();

            points[i].position = Common::Helix_PointFromUpAxis(up_axis, t, a, radius, height);
            points[i].color    = color;
        }
    }

    glm::f32vec3 CalculateAABBCenter(const AABB& aabb)
    {
        return (aabb.corner_min + aabb.corner_max) * 0.5f;
    }

    glm::f32vec3 CalculateAABBSize(const AABB& aabb)
    {
        return (aabb.corner_max - aabb.corner_min);
    }

    void ComputeAABB(const std::vector<Point3D>& points, AABB& out_aabb)
    {
        if (points.empty())
        {
            out_aabb.corner_min = glm::f32vec3(0.0f);
            out_aabb.corner_max = glm::f32vec3(0.0f);
            return;
        }

        glm::f32vec3 minV(std::numeric_limits<float>::max());
        glm::f32vec3 maxV(std::numeric_limits<float>::lowest());

        for (const auto& p : points)
        {
            minV = glm::min(minV, p.position);
            maxV = glm::max(maxV, p.position);
        }

        out_aabb.corner_min = minV;
        out_aabb.corner_max = maxV;
    }

    void ComputeAABB(const std::vector<ColorPoint3D>& points, AABB& out_aabb)
    {
        if (points.empty())
        {
            out_aabb.corner_min = glm::f32vec3(0.0f);
            out_aabb.corner_max = glm::f32vec3(0.0f);
            return;
        }

        glm::f32vec3 minV(std::numeric_limits<float>::max());
        glm::f32vec3 maxV(std::numeric_limits<float>::lowest());

        for (const auto& p : points)
        {
            minV = glm::min(minV, p.position);
            maxV = glm::max(maxV, p.position);
        }

        out_aabb.corner_min = minV;
        out_aabb.corner_max = maxV;
    }

    bool IsPointInsideAABB(const AABB& aabb, const glm::f32vec3& point)
    {
        return (point.x >= aabb.corner_min.x && point.x <= aabb.corner_max.x) &&
               (point.y >= aabb.corner_min.y && point.y <= aabb.corner_max.y) &&
               (point.z >= aabb.corner_min.z && point.z <= aabb.corner_max.z);
    }

    void GenerateAABBWireframePoints(std::vector<Point3D>& points, const AABB& aabb)
    {
        const glm::vec3& min = aabb.corner_min;
        const glm::vec3& max = aabb.corner_max;

        points = {
            {{min.x, min.y, min.z}},
            {{max.x, min.y, min.z}},
            {{max.x, min.y, min.z}},
            {{max.x, max.y, min.z}},
            {{max.x, max.y, min.z}},
            {{min.x, max.y, min.z}},
            {{min.x, max.y, min.z}},
            {{min.x, min.y, min.z}},

            {{min.x, min.y, max.z}},
            {{max.x, min.y, max.z}},
            {{max.x, min.y, max.z}},
            {{max.x, max.y, max.z}},
            {{max.x, max.y, max.z}},
            {{min.x, max.y, max.z}},
            {{min.x, max.y, max.z}},
            {{min.x, min.y, max.z}},

            {{min.x, min.y, min.z}},
            {{min.x, min.y, max.z}},
            {{max.x, min.y, min.z}},
            {{max.x, min.y, max.z}},
            {{max.x, max.y, min.z}},
            {{max.x, max.y, max.z}},
            {{min.x, max.y, min.z}},
            {{min.x, max.y, max.z}}};
    }

    void GenerateAABBWireframeColorPoints(std::vector<ColorPoint3D>& points, const AABB& aabb, const glm::u8vec4& color)
    {
        const glm::vec3& min = aabb.corner_min;
        const glm::vec3& max = aabb.corner_max;

        points = {
            {{min.x, min.y, min.z}, color},
            {{max.x, min.y, min.z}, color},
            {{max.x, min.y, min.z}, color},
            {{max.x, max.y, min.z}, color},
            {{max.x, max.y, min.z}, color},
            {{min.x, max.y, min.z}, color},
            {{min.x, max.y, min.z}, color},
            {{min.x, min.y, min.z}, color},

            {{min.x, min.y, max.z}, color},
            {{max.x, min.y, max.z}, color},
            {{max.x, min.y, max.z}, color},
            {{max.x, max.y, max.z}, color},
            {{max.x, max.y, max.z}, color},
            {{min.x, max.y, max.z}, color},
            {{min.x, max.y, max.z}, color},
            {{min.x, min.y, max.z}, color},

            {{min.x, min.y, min.z}, color},
            {{min.x, min.y, max.z}, color},
            {{max.x, min.y, min.z}, color},
            {{max.x, min.y, max.z}, color},
            {{max.x, max.y, min.z}, color},
            {{max.x, max.y, max.z}, color},
            {{min.x, max.y, min.z}, color},
            {{min.x, max.y, max.z}, color}};
    }

    void GenerateAABBFacePoints(std::vector<Point3D>& points, const AABB& aabb)
    {
        const glm::vec3& min = aabb.corner_min;
        const glm::vec3& max = aabb.corner_max;

        const glm::vec3 v000 = {min.x, min.y, min.z};
        const glm::vec3 v001 = {min.x, min.y, max.z};
        const glm::vec3 v010 = {min.x, max.y, min.z};
        const glm::vec3 v011 = {min.x, max.y, max.z};
        const glm::vec3 v100 = {max.x, min.y, min.z};
        const glm::vec3 v101 = {max.x, min.y, max.z};
        const glm::vec3 v110 = {max.x, max.y, min.z};
        const glm::vec3 v111 = {max.x, max.y, max.z};

        points = {
            {v000},
            {v010},
            {v110},
            {v000},
            {v110},
            {v100},

            {v001},
            {v101},
            {v111},
            {v001},
            {v111},
            {v011},

            {v000},
            {v100},
            {v101},
            {v000},
            {v101},
            {v001},

            {v010},
            {v011},
            {v111},
            {v010},
            {v111},
            {v110},

            {v000},
            {v001},
            {v011},
            {v000},
            {v011},
            {v010},

            {v100},
            {v110},
            {v111},
            {v100},
            {v111},
            {v101}};
    }

    void GenerateAABBFaceColorPoints(std::vector<ColorPoint3D>& points, const AABB& aabb, const glm::u8vec4& color)
    {
        const glm::vec3& min = aabb.corner_min;
        const glm::vec3& max = aabb.corner_max;

        const glm::vec3 v000 = {min.x, min.y, min.z};
        const glm::vec3 v001 = {min.x, min.y, max.z};
        const glm::vec3 v010 = {min.x, max.y, min.z};
        const glm::vec3 v011 = {min.x, max.y, max.z};
        const glm::vec3 v100 = {max.x, min.y, min.z};
        const glm::vec3 v101 = {max.x, min.y, max.z};
        const glm::vec3 v110 = {max.x, max.y, min.z};
        const glm::vec3 v111 = {max.x, max.y, max.z};

        points = {
            {v000, color},
            {v010, color},
            {v110, color},
            {v000, color},
            {v110, color},
            {v100, color},

            {v001, color},
            {v101, color},
            {v111, color},
            {v001, color},
            {v111, color},
            {v011, color},

            {v000, color},
            {v100, color},
            {v101, color},
            {v000, color},
            {v101, color},
            {v001, color},

            {v010, color},
            {v011, color},
            {v111, color},
            {v010, color},
            {v111, color},
            {v110, color},

            {v000, color},
            {v001, color},
            {v011, color},
            {v000, color},
            {v011, color},
            {v010, color},

            {v100, color},
            {v110, color},
            {v111, color},
            {v100, color},
            {v111, color},
            {v101, color}};
    }

    void GenerateAABBIndexed(std::vector<Point3D>& points, std::vector<uint32_t>& indices, const AABB& aabb)
    {
        const glm::vec3& min = aabb.corner_min;
        const glm::vec3& max = aabb.corner_max;

        points = {
            Point3D{{min.x, min.y, min.z}},
            Point3D{{max.x, min.y, min.z}},
            Point3D{{max.x, max.y, min.z}},
            Point3D{{min.x, max.y, min.z}},
            Point3D{{min.x, min.y, max.z}},
            Point3D{{max.x, min.y, max.z}},
            Point3D{{max.x, max.y, max.z}},
            Point3D{{min.x, max.y, max.z}}};

        indices = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7};
    }

    void GenerateAABBColorIndexed(std::vector<ColorPoint3D>& points, std::vector<uint32_t>& indices, const AABB& aabb, const glm::u8vec4& color)
    {
        const glm::vec3& min = aabb.corner_min;
        const glm::vec3& max = aabb.corner_max;

        points = {
            ColorPoint3D{{min.x, min.y, min.z}, color},
            ColorPoint3D{{max.x, min.y, min.z}, color},
            ColorPoint3D{{max.x, max.y, min.z}, color},
            ColorPoint3D{{min.x, max.y, min.z}, color},
            ColorPoint3D{{min.x, min.y, max.z}, color},
            ColorPoint3D{{max.x, min.y, max.z}, color},
            ColorPoint3D{{max.x, max.y, max.z}, color},
            ColorPoint3D{{min.x, max.y, max.z}, color}};

        indices = {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7};
    }
}
