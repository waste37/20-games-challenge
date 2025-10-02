#include <iostream>
#include <memory>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <unordered_map>
#include <thread>
#include "Game.h"

static Game game{};

int main()
{
    std::ios::sync_with_stdio(false);
    auto platform = std::make_shared<Platform>((int)game.WindowWidth, (int)game.WindowHeight, "PONG");
    game.Load(platform->Loader);
    game.ChangeState<StartState>();
    platform->Renderer.SetColor(0);

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    while (!platform->Window.ShouldClose() && !game.ShouldQuit) {
        start = std::chrono::high_resolution_clock::now();
        auto frame_end = start + std::chrono::microseconds(16666);
        game.HandleInput(platform->Window);
        game.Update();
        platform->BeginDrawing();
        game.Draw(platform->Renderer);
        platform->EndDrawing();
        std::this_thread::sleep_until(frame_end - std::chrono::microseconds(1000));
        while (std::chrono::high_resolution_clock::now() < frame_end)
            /* make sure the scheduler doesn't get in my way */;
    }
}