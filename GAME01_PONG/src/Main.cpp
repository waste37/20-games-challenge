#include <iostream>
#include <memory>
#include <cstdlib>

#include <Platform.h>
#include <geom/Geometry.h>
#include <gfx/GpuBuffer.h>
#include <gfx/VertexAttribArray.h>
#include <gfx/Shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static const char *vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec2 pos;\n"
"uniform mat4 projection;"
"void main() {\n"
"   gl_Position = projection * vec4(pos, -1.0, 1.0);\n"
"}\n";

static const char *fragment_shader =
"#version 330 core\n"
"uniform vec4 color;\n"
"void main() {\n"
"   gl_FragColor = color;\n"
"}\n";

struct Sprite {
    geom::Vec2<float> pos;
    geom::Vec2<float> size;
    geom::Color color;
};

static std::array<float, 6 * 2> SpriteData(const Sprite &s)
{
    return {
        s.pos.x(),              s.pos.y() + s.size.y(),
        s.pos.x() + s.size.x(), s.pos.y(),
        s.pos.x(),              s.pos.y(),
        s.pos.x(),              s.pos.y() + s.size.y(),
        s.pos.x() + s.size.x(), s.pos.y() + s.size.y(),
        s.pos.x() + s.size.x(), s.pos.y(),
    };
}
//void ReadFile()
//{
//    auto size = std::filesystem::file_size(path);
//    const char vertex_shader_source = std::string(size, '\0');
//    std::ifstream in(m_VertexShaderPath);
//    in.read(&m_VertexShaderPath[0], size);
//}

int main()
{
    geom::Vec<float, 2> u{ 1, 2 };
    geom::Vec<float, 2> v{ 2.0, 1.0 };

    auto result = u + v;
    std::cout << result.x() << " " << result.y();

    auto glfw_handle = glfw::Init();
    glfw::WindowHints hints{
        .Profile = glfw::OpenGlProfile::Core,
        .ContextVersionMajor = 3,
        .ContextVersionMinor = 3,
    };
    hints.Apply();

    glfw::Window window{ 800, 600, "PONG" };
    glfw::MakeContextCurrent(window);
    glad::InitGLLoader((glad::LoadProc)glfw::GetProcAddress);

    gfx::ShaderProgram program{ vertex_shader, fragment_shader };
    program.Build();
    auto projection = geom::OrtographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    /*for (size_t i = 0; i < projection.Rows; ++i) {
        std::cout << std::endl;
        for (size_t j = 0; j < projection.Cols; ++j) {
            std::cout << " " << projection[j][i];
        }
    }*/

    gfx::VertexAttribArray va{};
    gfx::GpuBuffer vb{ gfx::GpuBuffer::Target::ArrayBuffer };

    Sprite sprite = {
        {100, 100},
        {150, 150},
        {1.0, 1.0, 1.0, 1.0}
    };

    auto data = SpriteData(sprite);
    std::cout << "size: " << data.size();
    for (float x : data) {
        std::cout << " " << x;
    }

    vb.Bind();
    vb.SetData(data.data(), data.size() * sizeof(float), gfx::GpuBuffer::Usage::StaticDraw);
    va.Bind();
    va.SetAttribute(0, 2, GL_FLOAT, 2 * sizeof(float), 0);
    vb.Unbind();
    va.Unbind();

    while (!window.ShouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //program["projection"] = projection;
        program["projection"] = projection;
        program["color"] = sprite.color;
        program.Use();
        va.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        va.Unbind();

        window.SwapBuffers();
        glfw::PollEvents();
    }
}
