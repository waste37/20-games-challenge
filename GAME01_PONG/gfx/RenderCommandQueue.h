#pragma once


#include <memory>
#include <vector>

#include <glad/glad.h>

#include "Texture.h"
#include "SpriteStorage.h"
#include "Shader.h"

namespace gfx
{

struct RenderCommand {
    Texture Texture;
    GLenum Mode;
    std::vector<int> First;
    std::vector<GLsizei> Count;
    GLsizei DrawCount;
};

class RenderCommandQueue {
public:
    struct RenderCommandProxy {
        GLenum Mode;
        const int* First;
        const GLsizei* Count;
        GLsizei DrawCount;
    };

    constexpr void Push(const RenderCommand& command)
    {
        m_Textures.push_back(command.Texture);
        m_Modes.push_back(command.Mode);
        m_Firsts.insert(m_Firsts.end(), command.First.begin(), command.First.end());
        m_Counts.insert(m_Counts.end(), command.Count.begin(), command.Count.end());
        m_DrawCounts.push_back(command.DrawCount);
        m_ParameterOffsets.push_back(m_ParameterOffsets.back() + command.DrawCount);
        m_Size++;
    }

    constexpr size_t Size() const noexcept
    {
        return m_Size;
    }

    constexpr RenderCommandProxy Parameters(size_t i) const noexcept
    {
        return RenderCommandProxy{
            .Mode = m_Modes[i],
            .First = m_Firsts.data() + m_ParameterOffsets[i],
            .Count = m_Counts.data() + m_ParameterOffsets[i],
            .DrawCount = m_DrawCounts[i]
        };
    }

    constexpr gfx::ShaderId Shader(size_t i) const noexcept
    {
        return m_ShaderIds[i];
    }

    constexpr const gfx::Texture& Texture(size_t i) const noexcept
    {
        return m_Textures[i];
    }

private:
    size_t m_Size{ 0 };
    std::vector<ShaderId> m_ShaderIds;
    std::vector<gfx::Texture> m_Textures;
    std::vector<GLenum> m_Modes;
    std::vector<int> m_Firsts; // sequential bucket of subarrays, index inside it with drawcounts.
    std::vector<GLsizei> m_Counts; // sequential bucket of subarrays, index inside it with drawcounts.
    std::vector<GLsizei> m_DrawCounts;
    std::vector<size_t> m_ParameterOffsets{ 0 };
};

}