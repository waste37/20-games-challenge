#include <iostream>
#include <memory>
#include <cstdlib>

#include "Platform.h"
#include "geom/Geometry.h"
#include "gfx/GpuHandle.h"
#include "gfx/Shader.h"
#include "gfx/Renderer.h"


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



//void ReadFile()
//{
//    auto size = std::filesystem::file_size(path);
//    const char vertex_shader_source = std::string(size, '\0');
//    std::ifstream in(m_VertexShaderPath);
//    in.read(&m_VertexShaderPath[0], size);
//}

struct Sprites {

    geom::Vec2<float> Positions[100];
    geom::Vec2<float> Sizes[100];
    geom::Color Colors[10];

};

int main()
{

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

    gfx::Renderer renderer{ window };
    gfx::ShaderProgram program{ vertex_shader, fragment_shader };
    program.Build();

    renderer.SetColor(0x000000ff);
    while (!window.ShouldClose()) {
        renderer.Clear();
        renderer.DrawSprite(Bbox{100, 100, 150, 150}, 0xffff00ff);
        renderer.Flush();
        window.SwapBuffers();
        glfw::PollEvents();
    }
}
