#pragma once

#include <array>
#include <vector>

#include <glad/glad.h>

namespace gfx
{

class GpuBuffer {
public:
    enum class Target {
        ArrayBuffer = GL_ARRAY_BUFFER, 	                        //Vertex attributes
        CopyReadBuffer = GL_COPY_READ_BUFFER, 	                //Buffer copy source
        CopyWriteBuffer = GL_COPY_WRITE_BUFFER, 	            //Buffer copy destination
        ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER, 	        //Vertex array indices
        PixelPackBuffer = GL_PIXEL_PACK_BUFFER, 	            //Pixel read target
        PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER, 	        //Texture data source
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

    explicit GpuBuffer(Target target);
    ~GpuBuffer();
    void Bind() const noexcept;
    void Unbind() const noexcept;

    void Allocate(size_t capacity_bytes, Usage usage) const noexcept;

    template <typename T>
    void SetData(T* data, size_t element_count, size_t offset = 0) const noexcept
    {
        glBufferSubData(static_cast<GLenum>(m_Target), offset, element_count * sizeof(T), data);
    }

    template <typename T>
    void SetData(std::vector<T> v, size_t offset = 0) const noexcept
    {
        glBufferSubData(static_cast<GLenum>(m_Target), offset, v.size() * sizeof(T), v.data());
    }
    template <typename T, size_t N>
    void SetData(std::array<T, N> a, size_t offset = 0) const noexcept
    {
        glBufferSubData(static_cast<GLenum>(m_Target), offset, N * sizeof(T), a.data());
    }
private:
    GLuint m_Id;
    Target m_Target;
};

} // gfx
