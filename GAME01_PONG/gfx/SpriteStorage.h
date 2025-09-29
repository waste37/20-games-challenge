#pragma once

#include <vector>
#include <glad/glad.h>

#include "../core/multivector.h"
#include "../math/math.h"
#include "Sprite.h"

namespace gfx
{


template <size_t MaxSprites>
class SpriteStorage {
public:
    SpriteStorage() : m_Storage{ MaxSprites } {}

    constexpr static size_t FloatsPerSprite()
    {
        return VerticesPerSprite() * 5ull;
    }

    constexpr static size_t VerticesPerSprite()
    {
        return 4ull;
    }

    constexpr bool InSameBucket(size_t i, size_t j) const noexcept
    {
        const auto& a_type = m_Storage.get<2>()[i];
        const auto& a_texture = m_Storage.get<1>()[i].GetId();
        const auto& b_type = m_Storage.get<2>()[j];
        const auto& b_texture = m_Storage.get<1>()[j].GetId();

        return a_type == b_type && a_texture == b_texture;
    }

    void Sort()
    {
        m_Storage.sort([](const auto& a, const auto& b) {
            unsigned int a_texture = std::get<1>(a).GetId();
            unsigned int b_texture = std::get<1>(b).GetId();

            if (a_texture != b_texture) {
                return a_texture < b_texture;
            }

            SpriteType a_type = std::get<2>(a);
            SpriteType b_type = std::get<2>(b);
            if (a_type != b_type) {
                return a_type < b_type;
            }

            return &a < &b;
        });
    }

    constexpr void AddSprite(const Sprite& sprite)
    {
        m_Storage.push_back(sprite.Depth, sprite.Texture, sprite.Type, sprite.Bbox);
    }

    constexpr size_t Size() const noexcept
    {
        return m_Storage.size();
    }

    constexpr float Depth(size_t i) const
    {
        return m_Storage.get<0>()[i];
    }

    constexpr gfx::Texture Texture(size_t i) const
    {
        return m_Storage.get<1>()[i];
    }

    constexpr SpriteType Type(size_t i) const
    {
        return m_Storage.get<2>()[i];
    }

    constexpr math::Bbox Bbox(size_t i) const
    {
        return m_Storage.get<3>()[i];
    }

    constexpr void Clear() noexcept
    {
        m_Storage.clear();
    }

private:
    // depth, texture, type, bbox
    core::multivector<float, gfx::Texture, SpriteType, math::Bbox> m_Storage;
};

}
