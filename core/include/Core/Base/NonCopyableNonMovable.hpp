#pragma once

namespace Core
{
    class NonCopyableNonMovable
    {
    protected:
        NonCopyableNonMovable()  = default;
        ~NonCopyableNonMovable() = default;

    public:
        NonCopyableNonMovable(const NonCopyableNonMovable&)            = delete;
        NonCopyableNonMovable& operator=(const NonCopyableNonMovable&) = delete;

        NonCopyableNonMovable(NonCopyableNonMovable&&)            = delete;
        NonCopyableNonMovable& operator=(NonCopyableNonMovable&&) = delete;
    };
}
