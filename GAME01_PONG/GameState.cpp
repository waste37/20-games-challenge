#include "Game.h"

void StartState::Exit(Game& game)
{
    game.Reset();
}

void StartState::Draw(Game& game, gfx::Renderer<>& renderer)
{
    renderer.DrawSprite(game.Sprites[game.NameToId["field"]]);
}

void StartState::HandleInput(Game& game, glfw::Window& window)
{
    if (window.GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
        game.ChangeState<PlayState>();
    }
}

void PlayState::Update(Game& game)
{
    game.UpdatePositions();
    game.HandleCollisions();
    if (game.Player1Score >= 10 || game.Player2Score >= 10) {
        game.ChangeState<PlayerWonState>();
    }
}

void PlayState::Draw(Game& game, gfx::Renderer<>& renderer)
{
    for (ptrdiff_t i = game.NameToId["field"]; i >= 0; --i) {
        renderer.DrawSprite(game.Sprites[i]);
    }
}

void PlayState::HandleInput(Game& game, glfw::Window& window)
{
    if (window.GetKey(GLFW_KEY_P) == GLFW_PRESS) {
        game.ChangeState<PauseState>();
    }

    game.SetVelocity("player1", math::Vec2<float>(0.0f));
    if (window.GetKey(GLFW_KEY_S) == GLFW_PRESS) {
        game.SetVelocity("player1", { 0.0f, Game::PaddleSpeed });
    }
    if (window.GetKey(GLFW_KEY_W) == GLFW_PRESS) {
        game.SetVelocity("player1", { 0.0f, -Game::PaddleSpeed });
    }

    game.SetVelocity("player2", math::Vec2<float>(0.0f));
    if (window.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
        game.SetVelocity("player2", { 0.0f, Game::PaddleSpeed });
    }
    if (window.GetKey(GLFW_KEY_UP) == GLFW_PRESS) {
        game.SetVelocity("player2", { 0.0f, -Game::PaddleSpeed });
    }
}

void PlayerWonState::Exit(Game& game)
{
    game.Reset();
}

void PlayerWonState::Draw(Game& game, gfx::Renderer<>& renderer)
{
    renderer.DrawSprite(game.Sprites[game.NameToId["field"]]);
}

void PlayerWonState::HandleInput(Game& game, glfw::Window& window)
{
    if (window.GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
        game.ChangeState<PlayState>();
    }
}

void PauseState::Draw(Game& game, gfx::Renderer<>& renderer)
{
    for (ptrdiff_t i = game.NameToId["field"]; i >= 0; --i) {
        renderer.DrawSprite(game.Sprites[i]);
    }
}

void PauseState::HandleInput(Game& game, glfw::Window& window)
{
    if (window.GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
        game.ChangeState<PlayState>();
    }

}
