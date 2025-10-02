#pragma once

#include "Platform.h"

struct Game;

class IGameState {
public:
    virtual void Enter(Game& game) {}
    virtual void Exit(Game& game) {}
    virtual void Update(Game& game) {}
    virtual void Draw(Game& game, gfx::Renderer<>& renderer) {}
    virtual void HandleInput(Game& game, glfw::Window& window) {}
    virtual ~IGameState() {}
};

class StartState : public IGameState {
public:
    void Exit(Game& game) override;
    void Draw(Game& game, gfx::Renderer<>& renderer) override;
    void HandleInput(Game& game, glfw::Window& window) override;
};

class PlayState : public IGameState {
public:
    void Update(Game& game) override;
    void Draw(Game& game, gfx::Renderer<>& renderer) override;
    void HandleInput(Game& game, glfw::Window& window) override;
};

class PlayerWonState : public IGameState {
public:
    void Exit(Game& game) override;
    void Draw(Game& game, gfx::Renderer<>& renderer) override;
    void HandleInput(Game& game, glfw::Window& window) override;

};

class PauseState : public IGameState {
public:
    void Draw(Game& game, gfx::Renderer<>& renderer) override;
    void HandleInput(Game& game, glfw::Window& window) override;
};