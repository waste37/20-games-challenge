#pragma once 

#include <array>
#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../geom/Geometry.h"

#include "GpuBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommandQueue.h"


namespace gfx
{

using ShaderId = int;

/* Manages GPU and loads data into it */
class GpuHandle {
public:
    GpuHandle(size_t capacity) :
        m_Size{ 0 },
        m_Capacity{ capacity },
        m_VertexArray{},
        m_VertexBuffer{ gfx::GpuBuffer::Target::ArrayBuffer }
    {
    }

    ShaderId RegisterShader(const char *vertex_shader_source, const char *fragment_shader_source)
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
        m_VertexBuffer.Bind();
        m_VertexArray.SetAttribute(0, 2, GL_FLOAT, 2 * sizeof(float), 0);
        m_VertexArray.Unbind();
        m_VertexBuffer.Unbind();
    }

    bool UploadSpritesData(const std::vector<float> &data)
    {
        if (m_Size >= m_Capacity) {
            return false;
        }
        // migt need bind unbind va

        m_VertexBuffer.Bind();
        m_VertexBuffer.SetData(data, m_Size);
        m_VertexBuffer.Unbind();
        m_Size += data.size();
        return true;
    }

    void UploadCommandData(const RenderCommandQueue &queue)
    {
        for (size_t i = 0; i < queue.Size(); ++i) {
            //auto shader_id = queue.Shader(i);
            //auto shader_program = shaders[shader_id];
            m_ShaderProgram.Use();
            m_ShaderProgram["projection"] = queue.Projection(i);
            m_ShaderProgram["color"] = queue.Color(i);
            auto parameters = queue.Parameters(i);
            m_VertexBuffer.Bind();
            glMultiDrawArrays(parameters.Mode, parameters.First, parameters.Count, parameters.DrawCount);
            m_VertexBuffer.Unbind();
        }
    }

    void Clear(const geom::Color &color) noexcept
    {
        glClearColor(color.r(), color.g(), color.b(), color.a());
        glClear(GL_COLOR_BUFFER_BIT);
        // free the gpu linear allocator :D
        m_Size = 0;
    }

private:
    size_t m_Capacity;
    size_t m_Size;
    gfx::VertexArray m_VertexArray;
    gfx::GpuBuffer m_VertexBuffer;
    gfx::ShaderProgram m_ShaderProgram;
};

} // gfx
