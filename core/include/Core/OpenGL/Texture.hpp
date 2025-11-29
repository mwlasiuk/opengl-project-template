#pragma once

#include <Core/Base/NonCopyableNonMovable.hpp>
#include <Core/Structures.hpp>

namespace Core
{
    class Texture
        : private NonCopyableNonMovable
    {
    private:
        struct TextureIMPL;

    private:
        TextureIMPL* _impl = nullptr;

    public:
        explicit Texture(const TextureSpecification& specification);
        ~Texture();

        [[nodiscard]] uint32_t                    GetID() const;
        [[nodiscard]] const TextureSpecification& GetTextureSpecification() const;

        void Clear();

        void UpLoadExternalImage(const void* const data);

        void SetParameterF32(const uint32_t parameter, const float value);
        void SetParameterS32(const uint32_t parameter, const int32_t value);
    };
}
