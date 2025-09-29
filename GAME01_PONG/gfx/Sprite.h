#pragma once

#include <limits>
#include <glad/glad.h>

#include "../math/math.h"
#include "Texture.h"

namespace gfx
{

enum class SpriteType : uint8_t {
    TexturedRect,
};

struct Sprite {
    Sprite() {}
    Sprite(math::Bbox bbox, const Texture& tex)
        : Bbox{ bbox }, Texture{ tex }
    {
    }

    math::Bbox Bbox;
    float Depth{ std::numeric_limits<float>::quiet_NaN() }; // initialize to NaN
    gfx::Texture Texture;
    SpriteType Type{ SpriteType::TexturedRect };
};

}
