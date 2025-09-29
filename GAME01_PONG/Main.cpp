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
    platform->Renderer.SetColor(0);
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    while (!platform->Window.ShouldClose() && game.State != GameState::Quit) {
        start = std::chrono::high_resolution_clock::now();

        game.HandleInput(platform->Window);
        game.Update();
        platform->BeginDrawing();
        game.Draw(platform->Renderer);
        platform->EndDrawing();
        end = std::chrono::high_resolution_clock::now();

        dt = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        if (dt < 16666) {
            std::this_thread::sleep_for(std::chrono::microseconds(16666 - dt));
        }
    }
}