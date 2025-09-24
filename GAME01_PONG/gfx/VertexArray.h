#pragma once

#include <glad/glad.h>

namespace gfx
{

class VertexArray {
public:
    explicit VertexArray();
    ~VertexArray();
    void Bind() const noexcept;
    void Unbind() const noexcept;
    void SetAttribute(unsigned int index, int size, GLenum type, GLsizei stride, size_t offset) const noexcept;
    void DisableAttribute(unsigned int index) const noexcept;

private:
    unsigned int m_Id;
};

} // gfx
