#pragma once

#include <memory>

#include "../Platform.h"
#include "Texture.h"
#include "../math/math.h"
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

static const char* vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"layout (location = 1) in vec2 uv;\n"
"uniform mat4 projection;\n"
"out vec2 out_uv;\n"
"void main() {\n"
"   gl_Position = projection * vec4(pos, 1.0);\n"
"   out_uv = uv;\n"
"}\n";

static const char* fragment_shader =
"#version 330 core\n"
"uniform sampler2D tex;\n"
"in vec2 out_uv;\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = texture(tex, out_uv);\n"
"}\n";

template <ptrdiff_t MaxSprites = 256>
class Renderer {
public:
    Renderer(glfw::Window& window)
        : m_Window{ window },
        m_GpuHandle{ MaxSprites * SpriteStorage<MaxSprites>::FloatsPerSprite() * sizeof(float) },
        m_Color{ 0x000000ff },
        m_Depth{ -static_cast<float>(MaxSprites) }
    {
        auto [width, height] = window.Size();
        m_GpuHandle.SetProjectionMatrix(math::OrthographicProjection(
            0.0f, static_cast<float>(width),
            0.0f, static_cast<float>(height),
            0.0f, static_cast<float>(MaxSprites)
        ));
        window.SizeEvent.SetHandler([this](glfw::Window& w, int width, int height) {
            glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
            m_GpuHandle.SetProjectionMatrix(math::OrthographicProjection(
                0.0f, static_cast<float>(width),
                0.0f, static_cast<float>(height),
                0.0f, static_cast<float>(MaxSprites)
            ));
        });
    }

    void Init()
    {
        m_GpuHandle.Allocate();
        m_GpuHandle.CreateShader(vertex_shader, fragment_shader);
    }

    constexpr void DrawSprite(Sprite sprite) noexcept
    {
        if (std::isnan(sprite.Depth)) {
            sprite.Depth = m_Depth += 1.0f;
        }

        m_Storage.AddSprite(sprite);
    }


    constexpr void DrawSpritesSameDepth(std::vector<Sprite>& sprites) noexcept
    {
        if (!sprites.size()) return;

        m_Depth += 1.0f;

        for (auto& sprite : sprites) {
            sprite.Depth = m_Depth;
            m_Storage.AddSprite(sprite);
        }
    }


    constexpr void DrawSprite(math::Bbox bbox, Texture texture) noexcept
    {
        auto sprite = Sprite{ .Bbox = bbox, .Texture = texture, .Type = SpriteType::TexturedRect ,.Depth = m_Depth += 1.0f };
        m_Storage.AddSprite(sprite);
    }

    constexpr void DrawSprite(math::Bbox bbox, Texture texture, float depth) noexcept
    {
        auto sprite = Sprite{ .Bbox = bbox, .Texture = texture, .Type = SpriteType::TexturedRect ,.Depth = depth };
        m_Storage.AddSprite(sprite);
    }

    constexpr void SetColor(math::Color color) noexcept
    {
        m_Color = color;
    }

    constexpr void Clear() noexcept
    {
        m_GpuHandle.Clear(m_Color);
    }

    constexpr void Flush()
    {
        auto converter = std::make_unique<GpuDataConverter<MaxSprites>>(m_Storage);
        m_GpuHandle.UploadVertexData(converter->VertexData());
        m_GpuHandle.UploadCommandData(converter->DrawingData());
        m_GpuHandle.Free();
        m_Storage.Clear();
        m_Depth = -static_cast<float>(MaxSprites);
    }

private:
    const glfw::Window& m_Window;
    GpuHandle m_GpuHandle;
    math::Color m_Color;
    math::Mat<float, 4, 4> m_Projection;
    SpriteStorage<MaxSprites> m_Storage;
    float m_Depth;
};


} // Graphics
