#pragma once

#include <unordered_map>
#include <thread>
#include "Platform.h"
#include "math/math.h"

enum class GameState {
    Start,
    Play,
    PlayerWon,
    Pause,
    Quit
};

struct Game {
    constexpr static float Margin = 20.0f, WindowWidth = 800.0f, WindowHeight = 600.0f;
    constexpr static int SpritesCount = 4;
    constexpr static int EntityCount = 3;
    constexpr static math::Bbox FieldBbox{ Margin, Margin, WindowWidth - 2 * Margin, WindowHeight - 2 * Margin };

    float BallSpeed = 15.0f;
    float PaddleSpeed = 10.0f;

    std::vector<gfx::Sprite> Sprites;
    core::multivector<math::Vec2<float>, math::Vec2<float>, math::Vec2<float>> Entities{ EntityCount };
    std::unordered_map<const char*, int> NameToId;
    int Player1Score = 0, Player2Score = 0;
    GameState State = GameState::Start;
    bool Started;

    void Load(fs::AssetLoader& loader);
    void HandleInput(glfw::Window& window);
    void Update();
    void Draw(gfx::Renderer<>& renderer);

    void UpdateStart();
    void DrawStart(gfx::Renderer<>& renderer);

    void UpdatePlay();
    void DrawPlay(gfx::Renderer<>& renderer);

    void UpdateWon();
    void DrawWon(gfx::Renderer<>& renderer);

    void UpdatePause();
    void DrawPause(gfx::Renderer<>& renderer);


    void StartGame();
    void RestartGame();
    void QuitGame();
    void ResetPositions();
    math::Bbox GetBbox(const char* name);
    math::Vec2<float> GetVelocity(const char* name);
    void SetVelocity(const char* name, math::Vec2<float> v);
    void UpdatePositions();
    void HandleCollisions();
};
