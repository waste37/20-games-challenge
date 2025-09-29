#pragma once

#include <cstdlib>
#include <glad/glad.h>

namespace gfx
{

struct Image {
    Image(int width, int height, unsigned char* data) : Width{ width }, Height{ height }, Data{ data } {}
    ~Image() { free(Data); }
    int Width;
    int Height;
    unsigned char* Data;
};

class Texture {
public:
    Texture() noexcept
    {
        glGenTextures(1, &m_Id);
    }

    constexpr unsigned int GetId() const noexcept
    {
        return m_Id;
    }

    constexpr void Allocate(int width, int height, unsigned char* data) const noexcept
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        Unbind();
    }

    constexpr void Allocate(const Image& img) const noexcept
    {
        Allocate(img.Width, img.Height, img.Data);
    }

    constexpr void Bind() const noexcept
    {
        glBindTexture(GL_TEXTURE_2D, m_Id);
    }

    constexpr void Unbind() const noexcept
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
private:
    unsigned int m_Id;
};

}