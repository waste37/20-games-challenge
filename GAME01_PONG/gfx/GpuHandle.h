#pragma once 

#include <array>
#include <memory>
#include <vector>
#include <cstdlib>
#include <iostream>

#include <glad/glad.h>

#include "../math/math.h"

#include "GpuBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommandQueue.h"


namespace gfx
{

/* Manages GPU and loads data into it */
class GpuHandle {
public:
    GpuHandle(size_t capacity) :
        m_Size{ 0 },
        m_Capacity{ capacity },
        m_VertexArray{},
        m_VertexBuffer{ gfx::GpuBuffer::Target::ArrayBuffer },
        m_Projection{}
    {
        std::cout << "enabling depth test\n";
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    ShaderId CreateShader(const char* vertex_shader_source, const char* fragment_shader_source)
    {
        // one day can generalize for multiple shaders, now its just one.
        m_ShaderProgram = ShaderProgram{ vertex_shader_source, fragment_shader_source };
        if (!m_ShaderProgram.Build()) return -1;
        return m_ShaderProgram.Id();
    }

    void Allocate()
    {
        m_VertexBuffer.Bind();
        m_VertexBuffer.Allocate(m_Capacity, gfx::GpuBuffer::Usage::DynamicDraw);

        m_VertexArray.Bind();
        m_VertexArray.SetAttribute(0, 3, GL_FLOAT, 5 * sizeof(float), 0);
        m_VertexArray.SetAttribute(1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));
        m_VertexArray.Unbind();
        m_VertexBuffer.Unbind();
    }

    template <size_t N>
    bool UploadVertexData(const std::array<float, N>& data)
    {
        if (m_Size >= m_Capacity) {
            return false;
        }

        //for (int i = 0; i < data.size(); ++i) {
        //    std::cout << data[i] << " ";
        //    if (i % 5 == 4) std::cout << std::endl;
        //}
        //exit(0);

        // migt need bind unbind va
        m_VertexBuffer.Bind();
        m_VertexBuffer.SetData(data, m_Size);
        m_VertexBuffer.Unbind();
        m_Size += data.size();
        return true;
    }

    constexpr void SetProjectionMatrix(const math::Mat<float, 4, 4>& proj)
    {
        m_Projection = proj;
    }

    constexpr void SetProjectionMatrix(math::Mat<float, 4, 4>&& proj)
    {
        m_Projection = std::move(proj);
    }

    void UploadCommandData(const RenderCommandQueue& queue)
    {
        m_ShaderProgram.Use();
        m_ShaderProgram["projection"] = m_Projection;


        for (size_t i = 0; i < queue.Size(); ++i) {
            //auto shader_id = queue.Shader(i);
            //auto shader_program = shaders[shader_id];
            const auto& tex = queue.Texture(i);
            auto parameters = queue.Parameters(i);

            m_VertexBuffer.Bind();
            m_VertexArray.Bind();
            tex.Bind();
            m_ShaderProgram["tex"] = 0;

            glMultiDrawArrays(parameters.Mode, parameters.First, parameters.Count, parameters.DrawCount);
            tex.Unbind();
            m_VertexArray.Unbind();
            m_VertexBuffer.Unbind();
        }
        // exit(1);
    }

    constexpr void Clear(const math::Color& color) noexcept
    {
        glClearColor(color.r(), color.g(), color.b(), color.a());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    constexpr void Free() noexcept
    {
        // free the gpu linear allocator :D

        m_Size = 0;
    }
private:
    size_t m_Capacity;
    size_t m_Size;

    gfx::VertexArray m_VertexArray;
    gfx::GpuBuffer m_VertexBuffer;
    gfx::ShaderProgram m_ShaderProgram;

    math::Mat<float, 4, 4> m_Projection;
};

} // gfx
