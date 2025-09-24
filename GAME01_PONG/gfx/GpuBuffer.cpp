#include "GpuBuffer.h"
namespace gfx
{

GpuBuffer::GpuBuffer(Target target)
    : m_Target(target)
{
    glGenBuffers(1, &m_Id);
}

GpuBuffer::~GpuBuffer()
{
    glDeleteBuffers(1, &m_Id);
}

void GpuBuffer::Bind() const noexcept
{
    glBindBuffer(static_cast<GLenum>(m_Target), m_Id);
}

void GpuBuffer::Unbind() const noexcept
{
    glBindBuffer(static_cast<GLenum>(m_Target), 0);
}

void GpuBuffer::Allocate(size_t capacity_bytes, Usage usage) const noexcept
{
    glBufferData(static_cast<GLenum>(m_Target), capacity_bytes, nullptr, static_cast<GLenum>(usage));
}

} // gfx
