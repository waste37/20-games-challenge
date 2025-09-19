#include <iostream>
#include <memory>
#include <cstdlib>

#include "Platform.h"
#include "Geometry.h"

namespace Geom = Geometry;

int main()
{
    Geom::Vec2f x{ 1, 2 };
    Geom::Vec2f y{ 2, 1 };
    auto result1 = x + y;
    std::cout << result1.x() << " " << result1.y();
    auto result2 = x + 10;
    std::cout << result2.x() << " " << result2.y();

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