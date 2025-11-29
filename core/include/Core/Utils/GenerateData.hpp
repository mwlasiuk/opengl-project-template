#pragma once

#include <Core/Structures.hpp>

namespace Core
{
    // glDrawArrays + GL_POINTS
    void GenerateRandomPointsUniformCube(std::vector<Point3D>& points, const glm::f32vec3& extent = {1.0f, 1.0f, 1.0f}, const size_t count = 1024);
    // glDrawArrays + GL_POINTS
    void GenerateRandomColorPointsUniformCube(std::vector<ColorPoint3D>& points, const glm::f32vec3& extent = {1.0f, 1.0f, 1.0f}, const size_t count = 1024);

    // glDrawArrays + GL_POINTS
    void GenerateRandomPointsGaussianCube(std::vector<Point3D>& points, const glm::f32vec3& extent = {1.0f, 1.0f, 1.0f}, const size_t count = 1024);
    // glDrawArrays + GL_POINTS
    void GenerateRandomColorPointsGaussianCube(std::vector<ColorPoint3D>& points, const glm::f32vec3& extent = {1.0f, 1.0f, 1.0f}, const size_t count = 1024);

    // glDrawArrays + GL_POINTS
    void GenerateRandomPointsUniformSphere(std::vector<Point3D>& points, const float radius = 0.5f, const size_t count = 1024);
    // glDrawArrays + GL_POINTS
    void GenerateRandomColorPointsUniformSphere(std::vector<ColorPoint3D>& points, const float radius = 0.5f, const size_t count = 1024);

    // glDrawArrays + GL_POINTS
    void GenerateRandomPointsGaussianSphere(std::vector<Point3D>& points, const float radius = 0.5f, const size_t count = 1024);
    // glDrawArrays + GL_POINTS
    void GenerateRandomColorPointsGaussianSphere(std::vector<ColorPoint3D>& points, const float radius = 0.5f, const size_t count = 1024);

    // glDrawElements + GL_LINES
    void GenerateCubePointsIndexed(std::vector<Point3D>& points, std::vector<uint32_t>& indices, const glm::f32vec3& extent = {1.0f, 1.0f, 1.0f});
    // glDrawElements + GL_LINES
    void GenerateCubeColorPointsIndexed(std::vector<ColorPoint3D>& points, std::vector<uint32_t>& indices, const glm::f32vec3& extent = {1.0f, 1.0f, 1.0f}, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawElements + GL_LINES
    void GenerateSpherePointsIndexed(std::vector<Point3D>& points, std::vector<uint32_t>& indices, const float radius = 0.5f, const uint32_t segments = 32, const uint32_t rings = 32);
    // glDrawElements + GL_LINES
    void GenerateSphereColorPointsIndexed(std::vector<ColorPoint3D>& points, std::vector<uint32_t>& indices, const float radius = 0.5f, const uint32_t segments = 32, const uint32_t rings = 32, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS
    void GenerateFibonacciSpherePoints(std::vector<Point3D>& points, const float radius = 0.5f, const size_t count = 1024);
    // glDrawArrays + GL_POINTS
    void GenerateFibonacciSphereColorPoints(std::vector<ColorPoint3D>& points, const float radius = 0.5f, const size_t count = 1024, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS
    void GenerateDiscPoints(std::vector<Point3D>& points, const float radius = 0.5f, const size_t count = 1024, const UpAxis up_axis = UpAxis::DEFAULT);
    // glDrawArrays + GL_POINTS
    void GenerateDiscColorPoints(std::vector<ColorPoint3D>& points, const float radius = 0.5f, const size_t count = 1024, const UpAxis up_axis = UpAxis::DEFAULT, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS /  GL_LINE_LOOP
    void GenerateQuadPoints(std::vector<Point3D>& points, const glm::f32vec2& extent = {1.0f, 1.0f}, const UpAxis up_axis = UpAxis::DEFAULT);
    // glDrawArrays + GL_POINTS /  GL_LINE_LOOP
    void GenerateQuadColorPoints(std::vector<ColorPoint3D>& points, const glm::f32vec2& extent = {1.0f, 1.0f}, const UpAxis up_axis = UpAxis::DEFAULT, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_TRIANGLES
    void GenerateQuadPointsTriangles(std::vector<Point3D>& points, const glm::f32vec2& extent = {1.0f, 1.0f}, const UpAxis up_axis = UpAxis::DEFAULT);
    // glDrawArrays + GL_TRIANGLES
    void GenerateQuadColorPointsTriangles(std::vector<ColorPoint3D>& points, const glm::f32vec2& extent = {1.0f, 1.0f}, const UpAxis up_axis = UpAxis::DEFAULT, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    void GenerateGradientImageR8U(uint8_t* const image, const uint32_t width, const uint32_t height);
    void GenerateGradientImageRG8U(glm::u8vec2* const image, const uint32_t width, const uint32_t height);
    void GenerateGradientImageRGB8U(glm::u8vec3* const image, const uint32_t width, const uint32_t height);
    void GenerateGradientImageRGBA8U(glm::u8vec4* const image, const uint32_t width, const uint32_t height);

    void ClearImageR8U(uint8_t* const image, const uint32_t width, const uint32_t height, const uint8_t value = 0);
    void ClearImageRG8U(glm::u8vec2* const image, const uint32_t width, const uint32_t height, const glm::u8vec2& value = {0, 0});
    void ClearImageRGB8U(glm::u8vec3* const image, const uint32_t width, const uint32_t height, const glm::u8vec3& value = {0, 0, 0});
    void ClearImageRGBA8U(glm::u8vec4* const image, const uint32_t width, const uint32_t height, const glm::u8vec4& value = {0, 0, 0, 255});

    void ApplyPointJitter(std::vector<Point3D>& points, const float amount);
    void ApplyColorPointJitter(std::vector<ColorPoint3D>& points, const float amount);

    void ApplyPointWarp(std::vector<Point3D>& points, void (*const fn)(const Point3D&));
    void ApplyColorPointWarp(std::vector<ColorPoint3D>& points, void (*const fn)(ColorPoint3D&));

    void NormalizePointCloud(std::vector<Point3D>& points, const glm::vec3& extent);
    void NormalizeColorPointCloud(std::vector<ColorPoint3D>& points, const glm::vec3& extent);

    void CenterAndScalePointCloud(std::vector<Point3D>& points, const float scale);
    void CenterAndScaleColorPointCloud(std::vector<ColorPoint3D>& points, const float scale);

    // glDrawArrays + GL_POINTS
    void GenerateLinePoints(std::vector<Point3D>& points, const glm::vec3& a, const glm::vec3& b, const size_t count = 1024);
    // glDrawArrays + GL_POINTS
    void GenerateLineColorPoints(std::vector<ColorPoint3D>& points, const glm::vec3& a, const glm::vec3& b, const size_t count = 1024, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS
    void GeneratePolylinePoints(std::vector<Point3D>& points, const std::vector<glm::vec3>& control_points, const size_t points_per_segment);
    // glDrawArrays + GL_POINTS
    void GeneratePolylineColorPoints(std::vector<ColorPoint3D>& points, const std::vector<glm::vec3>& control_points, const size_t points_per_segment, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS
    void GenerateCirclePoints(std::vector<Point3D>& points, const float radius, const size_t segments, const UpAxis up_axis = UpAxis::DEFAULT);
    // glDrawArrays + GL_POINTS
    void GenerateCircleColorPoints(std::vector<ColorPoint3D>& points, const float radius, const size_t segments, const UpAxis up_axis = UpAxis::DEFAULT, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS
    void GenerateSpiralPoints(std::vector<Point3D>& points, const float radius, const float turns, const size_t count = 1024, const UpAxis up_axis = UpAxis::DEFAULT);
    // glDrawArrays + GL_POINTS
    void GenerateSpiralColorPoints(std::vector<ColorPoint3D>& points, const float radius, const float turns, const size_t count = 1024, const UpAxis up_axis = UpAxis::DEFAULT, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_POINTS
    void GenerateHelixPoints(std::vector<Point3D>& points, const float radius, const float height, const float turns, const size_t count = 1024, const UpAxis up_axis = UpAxis::DEFAULT);
    // glDrawArrays + GL_POINTS
    void GenerateHelixColorPoints(std::vector<ColorPoint3D>& points, const float radius, const float height, const float turns, const size_t count = 1024, const UpAxis up_axis = UpAxis::DEFAULT, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // AABB calculation helpers
    [[nodiscard]] glm::f32vec3 CalculateAABBCenter(const AABB& aabb);
    [[nodiscard]] glm::f32vec3 CalculateAABBSize(const AABB& aabb);

    void ComputeAABB(const std::vector<Point3D>& points, AABB& out_aabb);
    void ComputeAABB(const std::vector<ColorPoint3D>& points, AABB& out_aabb);

    bool IsPointInsideAABB(const AABB& aabb, const glm::f32vec3& point);

    // glDrawArrays + GL_LINES
    void GenerateAABBWireframePoints(std::vector<Point3D>& points, const AABB& aabb);
    // glDrawArrays + GL_LINES
    void GenerateAABBWireframeColorPoints(std::vector<ColorPoint3D>& points, const AABB& aabb, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawArrays + GL_TRIANGLES
    void GenerateAABBFacePoints(std::vector<Point3D>& points, const AABB& aabb);
    // glDrawArrays + GL_TRIANGLES
    void GenerateAABBFaceColorPoints(std::vector<ColorPoint3D>& points, const AABB& aabb, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});

    // glDrawElements + GL_LINES
    void GenerateAABBIndexed(std::vector<Point3D>& points, std::vector<uint32_t>& indices, const AABB& aabb);
    // glDrawElements + GL_LINES
    void GenerateAABBColorIndexed(std::vector<ColorPoint3D>& points, std::vector<uint32_t>& indices, const AABB& aabb, const glm::u8vec4& color = {0XFF, 0XFF, 0XFF, 0XFF});
}
