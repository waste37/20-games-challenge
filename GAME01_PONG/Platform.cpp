#include "Platform.h"

#include <iostream>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



void glad::InitGLLoader(glad::LoadProc load_proc)
{
    if (!gladLoadGLLoader(load_proc)) {
        throw Error("Failed to initialize GLAD");
    }
}

namespace fs
{

gfx::Image AssetLoader::LoadImage(const char* path) const noexcept
{
    int width;
    int height;
    int components;
    unsigned char* data = stbi_load(path, &width, &height, &components, 4);

    if (data == nullptr) {
        std::cerr << "Could not load image " << path << std::endl;
        std::exit(37);
    }

    return gfx::Image{ width, height, data };
}

gfx::Sprite AssetLoader::LoadSpriteFromImage(const char* sprite_path) const noexcept
{
    auto image = LoadImage(sprite_path);
    std::cout << "loaded image " << sprite_path << ": " << image.Width << "x" << image.Height << std::endl;
    gfx::Texture texture{};
    texture.Allocate(image);
    std::cout << "Allocated texture\n";
    return gfx::Sprite{
        math::Bbox{0.0f, 0.0f, (float)image.Width, (float)image.Height},
        texture
    };
}
}

Platform::Platform(int width, int height, const char* name)
    : m_GLFWHandle{ glfw::Init() },
    m_WindowHints{
        glfw::WindowHints{
            .Profile = glfw::OpenGlProfile::Core,
            .ContextVersionMajor = 3,
            .ContextVersionMinor = 3
        }.Apply() },
    Window{ width, height, name },
    Renderer{ Window },
    Loader{}
{

    Renderer.Init();
}

void Platform::BeginDrawing()
{
    Renderer.Clear();
}

void Platform::EndDrawing()
{
    Renderer.Flush();
    Window.SwapBuffers();
    glfw::PollEvents();
}