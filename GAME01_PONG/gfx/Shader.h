#pragma once


#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <string_view>
#include <glad/glad.h>

namespace gfx
{

namespace detail
{
bool CheckShaderOperationResult(unsigned int id, GLenum what, const char *who)
{
    int success;
    char info_log[512];
    glGetShaderiv(id, what, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, info_log);
        std::cout << "ERROR: " << who << " failed : \n" << info_log << std::endl;
    };

    return success;
}

}

class Shader {
public:
    enum class Type {
        Vertex = GL_VERTEX_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

    Shader(std::string_view source, Type type)
        : m_Id{ glCreateShader(static_cast<GLenum>(type)) }, m_Source(source)
    {
    }

    ~Shader() { glDeleteShader(m_Id); }

    constexpr unsigned int GetId() const noexcept { return m_Id; }
    bool Compile() const noexcept
    {
        const char *source_ptr = m_Source.data();
        glShaderSource(m_Id, 1, &source_ptr, nullptr);
        glCompileShader(m_Id);

        return detail::CheckShaderOperationResult(m_Id, GL_COMPILE_STATUS, "Shader");
    }

private:
    unsigned int m_Id;
    std::string_view m_Source;
};


class ShaderProgram {

public:
    ShaderProgram(
        const char *vertex_shader_source,
        const char *fragment_shader_source
    ) : m_Id{ glCreateProgram() },
        m_VertexShaderSource{ vertex_shader_source },
        m_FragmentShaderSource{ fragment_shader_source }
    {
    }

    bool Build()
    {
        Shader vertex_shader{ m_VertexShaderSource, Shader::Type::Vertex };
        if (!vertex_shader.Compile()) return false;
        glAttachShader(m_Id, vertex_shader.GetId());

        Shader fragment_shader{ m_FragmentShaderSource, Shader::Type::Fragment };
        if (!fragment_shader.Compile()) return false;
        glAttachShader(m_Id, fragment_shader.GetId());

        glLinkProgram(m_Id);
        return detail::CheckShaderOperationResult(m_Id, GL_LINK_STATUS, "Linking");
    }

    void Use() const noexcept
    {
        glUseProgram(m_Id);
    }

    class ShaderUniform {
    public:
        ShaderUniform(unsigned int program_id, std::string_view uniform_name)
            : m_ProgId(program_id), m_UniformName(uniform_name)
        {
        }

        bool operator=(bool value) noexcept { glUniform1i(Location(), (int)value); }
        float operator=(float value) noexcept { glUniform1f(Location(), value); }
        int operator=(int value) noexcept { glUniform1i(Location(), value); }
    private:
        constexpr int Location() const noexcept
        {
            return glGetUniformLocation(m_ProgId, m_UniformName.data());
        }

        unsigned int m_ProgId;
        std::string_view m_UniformName;
    };

    class ConstShaderUniform {
    public:
        ConstShaderUniform(unsigned int program_id, std::string_view uniform_name)
            : m_ProgId(program_id), m_UniformName(uniform_name)
        {
        }

        operator bool() const noexcept
        {
            bool value{};
            glGetUniformiv(m_ProgId, Location(), (int *)&value);
            return value;
        }

        operator float() const noexcept
        {
            float value{};
            glGetUniformfv(m_ProgId, Location(), &value);
            return value;
        }

        operator int() const noexcept
        {
            int value{};
            glGetUniformiv(m_ProgId, Location(), &value);
            return value;
        }

    private:
        constexpr int Location() const noexcept
        {
            return glGetUniformLocation(m_ProgId, m_UniformName.data());
        }

        unsigned int m_ProgId;
        std::string_view m_UniformName;
    };

    ShaderUniform operator[](std::string_view uniform_name) noexcept
    {
        return ShaderUniform{ m_Id, uniform_name };
    }

    ConstShaderUniform operator[](std::string_view uniform_name) const noexcept
    {
        return ConstShaderUniform{ m_Id, uniform_name };
    }

private:
    unsigned int m_Id;
    std::string_view m_VertexShaderSource;
    std::string_view m_FragmentShaderSource;
    std::string_view m_GeometryShaderSource;
};

}
