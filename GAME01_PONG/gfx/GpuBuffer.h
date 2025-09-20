#pragma once

#include <glad/glad.h>

namespace gfx
{

class GpuBuffer {
public:
    enum class Target {
        ArrayBuffer = GL_ARRAY_BUFFER, 	                        //Vertex attributes
        // AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,         //Atomic counter storage
        CopyReadBuffer = GL_COPY_READ_BUFFER, 	                //Buffer copy source
        CopyWriteBuffer = GL_COPY_WRITE_BUFFER, 	            //Buffer copy destination
        // DispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,   //Indirect compute dispatch commands
        // DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER, 	        //Indirect command arguments
        ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER, 	        //Vertex array indices
        PixelPackBuffer = GL_PIXEL_PACK_BUFFER, 	            //Pixel read target
        PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER, 	        //Texture data source
        // QueryBuffer = GL_QUERY_BUFFER, 	                        //Query result buffer
        // ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER, 	    //Read-write storage for shaders
        TextureBuffer = GL_TEXTURE_BUFFER, 	                    //Texture data buffer
        TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER, //Transform feedback buffer
        UniformBuffer = GL_UNIFORM_BUFFER,                      //Uniform Block storage
    };

    enum class Usage {
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,
        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY,
    };

    explicit GpuBuffer(Target target)
        : m_Target(target)
    {
        glGenBuffers(1, &m_Id);
    }

    ~GpuBuffer()
    {
        glDeleteBuffers(1, &m_Id);
    }

    void Bind() const noexcept
    {
        glBindBuffer(static_cast<GLenum>(m_Target), m_Id);
    }

    void Unbind() const noexcept
    {
        glBindBuffer(static_cast<GLenum>(m_Target), 0);
    }

    void SetData(float *data, size_t byte_count, Usage usage) const noexcept
    {
        glBufferData(static_cast<GLenum>(m_Target), byte_count, data, static_cast<GLenum>(usage));
    }

    template <typename T>
    void SetData(std::vector<T> v, Usage usage) const noexcept
    {
        glBufferData(static_cast<GLenum>(m_Target), v.size() * sizeof(T), v.data(), static_cast<GLenum>(usage));
    }

    template <typename T, size_t N>
    void SetData(std::array<T, N> a, Usage usage) const noexcept
    {
        glBufferData(static_cast<GLenum>(m_Target), N * sizeof(T), a.data(), static_cast<GLenum>(usage));
    }

private:
    GLuint m_Id;
    Target m_Target;
};

} // gfx
