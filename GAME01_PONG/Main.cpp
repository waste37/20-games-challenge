#include <iostream>
#include <memory>
#include <cstdlib>

#include "Platform.h"
#include "geom/Geometry.h"
#include "gfx/GpuBuffer.h"
#include "gfx/VertexAttribArray.h"
#include "gfx/Shader.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static const char *vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec2 pos;\n"
"layout (location = 1) in vec2 uv;\n"
"uniform mat4 projection;"
"void main() {\n"
"   gl_Position = projection * vec4(pos, -1.0, 1.0);\n"
"}\n";

static const char *fragment_shader =
"#version 330 core\n"
"void main() {\n"
"   gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\n";




float vertices[] = {
    // pos      // tex
    0.0f, 100.0f, 0.0f, 1.0f,
    100.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 100.0f, 0.0f, 1.0f,
    100.0f, 100.0f, 1.0f, 1.0f,
    100.0f, 0.0f, 1.0f, 0.0f
};



//void readFile()
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
    program["projection"] = geom::OrtographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    /*for (size_t i = 0; i < projection.Rows; ++i) {
        std::cout << std::endl;
        for (size_t j = 0; j < projection.Cols; ++j) {
            std::cout << " " << projection[j][i];
        }
    }*/


    gfx::VertexAttribArray va{};
    gfx::GpuBuffer vb{ gfx::GpuBuffer::Target::ArrayBuffer };

    vb.Bind();
    vb.SetData(vertices, sizeof(vertices), gfx::GpuBuffer::Usage::StaticDraw);
    va.Bind();
    va.SetAttribute(0, 2, GL_FLOAT, 4 * sizeof(float), 0);
    va.SetAttribute(1, 2, GL_FLOAT, 4 * sizeof(float), 2 * sizeof(float));
    vb.Unbind();
    va.Unbind();

    while (!window.ShouldClose()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //program["projection"] = projection;
        program["projection"] = geom::OrtographicProjection(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        program.Use();
        va.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        va.Unbind();

        window.SwapBuffers();
        glfw::PollEvents();
    }
}
