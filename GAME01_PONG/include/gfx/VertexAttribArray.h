#pragma once

#include <glad/glad.h>

namespace gfx
{

class VertexAttribArray {
public:
    explicit VertexAttribArray()
    {
        glGenVertexArrays(1, &m_Id);
    }

    ~VertexAttribArray()
    {
        glDeleteVertexArrays(1, &m_Id);
    }

    void Bind() const noexcept
    {
        glBindVertexArray(m_Id);
    }

    void Unbind() const noexcept
    {
        glBindVertexArray(0);
    }

    void SetAttribute(unsigned int index, int size, GLenum type, GLsizei stride, size_t offset) const noexcept
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, reinterpret_cast<void *>(offset));
    }

    void DisableAttribute(unsigned int index) const noexcept
    {
        glDisableVertexAttribArray(index);
    }

private:
    unsigned int m_Id;
};

} // gfx
