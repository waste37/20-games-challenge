#include "VertexArray.h"

namespace gfx
{

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_Id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_Id);
}

void VertexArray::Bind() const noexcept
{
    glBindVertexArray(m_Id);
}

void VertexArray::Unbind() const noexcept
{
    glBindVertexArray(0);
}

void VertexArray::SetAttribute(unsigned int index, int size, GLenum type, GLsizei stride, size_t offset) const noexcept
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, GL_FALSE, stride, reinterpret_cast<void *>(offset));
}

void VertexArray::DisableAttribute(unsigned int index) const noexcept
{
    glDisableVertexAttribArray(index);
}

} // gfx
