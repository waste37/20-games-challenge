#pragma once

#include <vector>

#include <glad/glad.h>

#include "../geom/Geometry.h"

namespace gfx
{

enum class SpriteDrawingMode {
    Points = GL_POINTS,
    LineStrip = GL_LINE_STRIP,
    LineLoop = GL_LINE_LOOP,
    Lines = GL_LINES,
    LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
    LinesAdjacency = GL_LINES_ADJACENCY,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleFan = GL_TRIANGLE_FAN,
    Triangles = GL_TRIANGLES,
    TrianglesStringAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    TrianglesAdjacency = GL_TRIANGLES_ADJACENCY
};

struct SpriteBucketInfo {
    geom::Color Color;
};

struct SpriteStorage {

    constexpr static size_t FloatsPerSprite()
    {
        return 4ull * 2ull;
    }

    constexpr static bool InSameBucket(const SpriteBucketInfo &a, const SpriteBucketInfo &b) noexcept
    {
        return a.r() == b.r() && a.g() == b.g() && a.b() == b.b() && a.a() && b.a();
    }

    constexpr void AddSprite(geom::Bbox bbox, geom::Color color, SpriteDrawingMode mode) noexcept
    {
        BBoxes.push_back(bbox);
        Colors.push_back(color);
        Modes.push_back(modes);
    }

    constexpr SpriteBucketInfo operator[](size_t i) const noexcept 
    {
        return SpriteBucketInfo{ .Color = Colors[i] };
    }


    std::vector<geom::Bbox> BBoxes;
    std::vector<geom::Color> Colors;
    std::vector<SpriteDrawingMode> Modes;
};

}
