#include <iostream>
#include <memory>
#include <cstdlib>

#include "Platform.h"
#include "Geometry.h"

namespace Geom = Geometry;

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


    while (!window.ShouldClose()) {
        window.SwapBuffers();
        glfw::PollEvents();
    }
    return 0;
}