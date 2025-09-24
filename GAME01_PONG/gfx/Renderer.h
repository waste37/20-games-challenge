#pragma once

#include "../Platform.h"
#include "../geom/Geometry.h"
#include "GpuHandle.h"
#include "GpuDataConverter.h"
#include "SpriteStorage.h"

namespace gfx
{

/* The renderer class manages stuff until the flush operation.
 * The persistence of data between frames is handled by the user,
 * however its just about textures, the other stuff is recomputed each time.
 * The internal pipeline is: User sets up the renderer state with its public interface
 * Then he calls Renderer::Flush(). From now on the flow is passed to the Graphics module.
 * The module does the following:
 * Sorts the data based on shader and texture, or just shader. Notice that when there is no texture
 * We make as if there an empty one? Its just a different shader... However being 2D stuff, everything has a texture anyway.
 * The SoA that represents the sequence of things to draw can be easily managed?
 * */

namespace detail
{
size_t ComputeGpuMemoryPerSprite(size_t max_sprites)
{
    return max_sprites;
}

}

template <size_t MaxSprites = 256>
class Renderer {
public:
    Renderer(const glfw::Window &window)
        : m_Window{ window },
        m_GpuHandle{ MaxSprites * SpriteStorage::VertexPerSprite() * sizeof(float)) },
        m_Color{ 0x000000ff }
    {
    }

    constexpr void DrawSprite(geom::Bbox bbox, geom::Color color) noexcept
    {
        m_Storage.AddSprite(bbox, color);
    }

    constexpr void SetColor(geom::Color color) noexcept
    {
        m_Color = color;
    }

    constexpr void Clear() noexcept
    {
        m_GpuHandle.Clear(m_Color);
    }

    constexpr void Flush()
    {
        auto converter = GpuDataConverter<MaxSprites>(m_Storage);
        m_GpuHandle.UploadSpritesData(converter.SpritesData());
        m_GpuHandle.UploadCommandData(converter.DrawingData());
    }
private:
    GpuHandle m_GpuHandle;
    const glfw::Window &m_Window;
    geom::Color m_Color;
    SpriteStorage m_Storage;
};


} // Graphics
