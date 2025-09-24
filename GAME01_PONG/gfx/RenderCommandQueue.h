#pragma once


#include <memory>
#include <vector>

#include <glad/glad.h>

#include "Shader.h"

namespace gfx
{
class RenderCommandQueue {
public:
    struct DrawCallParameters {
        GLenum Mode;
        const int *First;
        const GLsizei *Count;
        size_t DrawCount;
    };

    constexpr size_t Size() const noexcept
    {
        return m_Size;
    }

    constexpr DrawCallParameters Parameters(size_t i) const noexcept
    {
        auto result = DrawCallParameters{
            .Mode = m_Modes[i],
            .First = m_Firsts.data(),
            .Count = m_Counts.data(),
            .DrawCount = m_DrawCounts[i]
        };

        if (i > 0) {
            result.First += m_DrawCounts[i - 1];
            result.Count += m_DrawCounts[i - 1];
        }

        return result;
    }

    constexpr gfx::ShaderId Shader(size_t i) const noexcept
    {
        return m_ShaderIds[i];
    }

    constexpr const geom::Mat<float, 4, 4> &Projection(size_t i) const noexcept
    {
        return m_Projection;
    }

    constexpr const geom::Color &Color(size_t i) const noexcept
    {
        return m_Colors[i];
    }

private:
    size_t m_Size;
    std::vector<ShaderId> m_ShaderIds;
    geom::Mat<float, 4, 4> m_Projection;
    std::vector<geom::Color> m_Colors;
    std::vector<GLenum> m_Modes;
    std::vector<int> m_Firsts; // sequential bucket of subarrays, index inside it with drawcounts.
    std::vector<GLsizei> m_Counts; // sequential bucket of subarrays, index inside it with drawcounts.
    std::vector<size_t> m_DrawCounts;
};

}