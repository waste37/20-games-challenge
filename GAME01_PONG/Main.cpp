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
    // renderer Init(max_data)
    constexpr size_t MAX_DATA = 100ull * 12ull * sizeof(float);
    gfx::GpuHandle gpu_uploader{ MAX_DATA };
    gpu_uploader.Allocate();

    gfx::ShaderProgram program{ vertex_shader, fragment_shader };
    program.Build();
    auto [width, height] = window.Size();
    auto projection = geom::OrtographicProjection(0.0f, (float)width, 0.0f, (float)height, -0.1f, 100.0f);
    program.Use();
    program["projection"] = projection;

    gfx::RenderCommandQueue commands;

    while (!window.ShouldClose()) {
        // Renderer::Clear();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Renderer::AddSprite(sprite, color);

        auto data = SpriteData(sprite);
        program["color"] = sprite.color;    // this is per bucket



        gpu_uploader.UploadSpritesData(data);
        gpu_uploader.UploadCommandData(commands);

        // Command Executor ? prende va?
        va.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        va.Unbind();

        window.SwapBuffers();
        glfw::PollEvents();
    }
}
