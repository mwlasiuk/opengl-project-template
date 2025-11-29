#pragma once

#include <cstdint>
#include <vector>

struct VertexBufferAttributeLayout
{
    uint32_t location;
    int32_t  components;
    uint32_t type;
    int32_t  normalize;
    int32_t  stride;
    int32_t  offset;
};

typedef std::vector<VertexBufferAttributeLayout> VertexBufferLayout;