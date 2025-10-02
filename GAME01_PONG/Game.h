#pragma once

#include <unordered_map>
#include <thread>
#include <type_traits>
#include <memory>

#include "Platform.h"
#include "math/math.h"
#include "GameState.h"

struct Game {
    constexpr static float Margin = 20.0f, WindowWidth = 800.0f, WindowHeight = 600.0f;
    constexpr static int SpritesCount = 4;
    constexpr static int EntityCount = 3;
    constexpr static math::Bbox FieldBbox{ Margin, Margin, WindowWidth - 2 * Margin, WindowHeight - 2 * Margin };
    constexpr static float BallSpeed = 15.0f;
    constexpr static float PaddleSpeed = 10.0f;

    std::unique_ptr<IGameState> CurrentState;
    std::vector<gfx::Sprite> Sprites;
    core::multivector<math::Vec2<float>, math::Vec2<float>, math::Vec2<float>> Entities{ EntityCount };
    std::unordered_map<const char*, int> NameToId;
    int Player1Score = 0, Player2Score = 0;
    bool ShouldQuit = false;

    template <typename StateT>
    void ChangeState()
    {
        static_assert(std::is_base_of_v<IGameState, StateT>, "T must be a subclass of IGameState");
        if (CurrentState) CurrentState->Exit(*this);
        CurrentState = std::make_unique<StateT>();
        if (CurrentState) CurrentState->Enter(*this);
    }

    void Reset();
    void Load(fs::AssetLoader& loader);
    void HandleInput(glfw::Window& window);
    void Update();
    void Draw(gfx::Renderer<>& renderer);

    void ResetPositions();
    const gfx::Sprite& GetSprite(const char* name);
    math::Bbox GetBbox(const char* name);
    math::Vec2<float> GetVelocity(const char* name);
    void SetVelocity(const char* name, math::Vec2<float> v);
    void UpdatePositions();
    void HandleCollisions();
};
