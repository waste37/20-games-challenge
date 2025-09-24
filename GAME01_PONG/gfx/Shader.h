#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include <glad/glad.h>

#include "../geom/Geometry.h"

namespace gfx
{

using ShaderId = int;

class Shader {
public:
    enum class Type {
        Vertex = GL_VERTEX_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

    Shader(std::string_view source, Type type);
    ~Shader();
    constexpr unsigned int GetId() const noexcept;
    bool Compile() const noexcept;
private:
    unsigned int m_Id;
    std::string_view m_Source;
};


class ShaderProgram {

public:
    ShaderProgram() = default;
    ShaderProgram(const char *vertex_shader_source, const char *fragment_shader_source);
    constexpr unsigned int Id() const noexcept { return m_Id; }
    bool Build() noexcept;
    void Use() const noexcept;
    class ShaderUniform {
    public:
        ShaderUniform(unsigned int program_id, std::string_view uniform_name);
        void operator=(bool value) noexcept;
        void operator=(float value) noexcept;
        void operator=(int value) noexcept;
        void operator=(geom::Mat<float, 4, 4> &&value) noexcept;
        void operator=(const geom::Mat<float, 4, 4> &value) noexcept;
        void operator=(geom::Vec<float, 4> &&value) noexcept;
        void operator=(const geom::Vec<float, 4> &value) noexcept;
        void operator=(geom::Vec<float, 3> &&value) noexcept;
        void operator=(const geom::Vec<float, 3> &value) noexcept;
        void operator=(geom::Vec<float, 2> &&value) noexcept;
        void operator=(const geom::Vec<float, 2> &value) noexcept;
    private:
        constexpr int Location() const noexcept;
        unsigned int m_ProgId;
        std::string_view m_UniformName;
    };

    class ConstShaderUniform {
    public:
        ConstShaderUniform(unsigned int program_id, std::string_view uniform_name);
        operator bool() const noexcept;
        operator float() const noexcept;
        operator int() const noexcept;

    private:
        constexpr int Location() const noexcept;
        unsigned int m_ProgId;
        std::string_view m_UniformName;
    };

    ShaderUniform operator[](std::string_view uniform_name) noexcept;

    ConstShaderUniform operator[](std::string_view uniform_name) const noexcept;

private:
    unsigned int m_Id;
    std::string_view m_VertexShaderSource;
    std::string_view m_FragmentShaderSource;
    std::string_view m_GeometryShaderSource;
};

}