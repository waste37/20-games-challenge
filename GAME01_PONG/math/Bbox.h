#pragma once

#include "Vec.h"

namespace math
{

struct Bbox {
    constexpr Bbox() = default;
    constexpr Bbox(float x, float y, float w, float h)
        : Pos{ x, y }, Size{ w, h }
    {
    }

    constexpr Bbox(Vec2<float> p, Vec2<float> s)
        : Pos{ p }, Size{ s }
    {
    }

    constexpr bool Contains(const Bbox& other) const noexcept
    {
        auto contains_x = other.Pos.x() > Pos.x() && other.Pos.x() + other.Size.x() < Pos.x() + Size.x();
        auto contains_y = other.Pos.y() > Pos.y() && other.Pos.y() + other.Size.y() < Pos.y() + Size.y();
        return contains_x && contains_y;
    }

    constexpr bool Intersects(const Bbox& other) const noexcept
    {
        return other.Pos.x() < Pos.x() + Size.x() && Pos.x() < other.Pos.x() + other.Size.x()
            && other.Pos.y() < Pos.y() + Size.y() && Pos.y() < other.Pos.y() + other.Size.y();
    }

    constexpr Vec2<float> Center() const noexcept
    {
        return { (Pos.x() + Pos.x() + Size.x()) / 2.0f, (Pos.y() + Pos.y() + Size.y()) / 2.0f };
    }

    math::Vec2<float> Pos;
    math::Vec2<float> Size;
};


constexpr bool operator==(const Bbox& a, const Bbox& b) noexcept
{
    return a.Pos == b.Pos && a.Size == b.Size;
}

}