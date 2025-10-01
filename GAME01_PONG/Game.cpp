#include "Game.h"

void Game::StartGame()
{
    if (State == GameState::Play) return;

    ResetPositions();
    SetVelocity("ball", math::RandomUnitVector<float, 2>() * BallSpeed);
    State = GameState::Play;
}

void Game::Load(fs::AssetLoader& loader)
{
    auto field = loader.LoadSpriteFromImage("Assets/field.png");
    auto ball = loader.LoadSpriteFromImage("Assets/ball.png");
    auto player1 = loader.LoadSpriteFromImage("Assets/paddle.png");
    auto player2 = loader.LoadSpriteFromImage("Assets/paddle.png");

    Sprites.push_back(ball);
    Sprites.push_back(player1);
    Sprites.push_back(player2);
    Sprites.push_back(field);

    Entities.push_back(std::make_tuple(math::Vec2<float>(0.0f), Sprites[0].Bbox.Size, math::Vec2<float>(0.0f)));
    Entities.push_back(std::make_tuple(math::Vec2<float>(0.0f), Sprites[1].Bbox.Size, math::Vec2<float>(0.0f)));
    Entities.push_back(std::make_tuple(math::Vec2<float>(0.0f), Sprites[2].Bbox.Size, math::Vec2<float>(0.0f)));

    NameToId["ball"] = 0;
    NameToId["player1"] = 1;
    NameToId["player2"] = 2;
    NameToId["field"] = 3;
    ResetPositions();
}

void Game::Update()
{
    switch (State) {
    case GameState::Start:
        UpdateStart();
        break;
    case GameState::Play:
        UpdatePlay();
        break;
    case GameState::PlayerWon:
        UpdateWon();
        break;
    case GameState::Pause:
        UpdatePause();
        break;
    default: break;
    }
}

void Game::Draw(gfx::Renderer<>& renderer)
{
    switch (State) {
    case GameState::Start:
        DrawStart(renderer);
        break;
    case GameState::Play:
        DrawPlay(renderer);
        break;
    case GameState::PlayerWon:
        DrawWon(renderer);
        break;
    case GameState::Pause:
        DrawPause(renderer);
        break;
    default: break;
    }
}

void Game::UpdateStart() {}
void Game::DrawStart(gfx::Renderer<>& renderer)
{
    renderer.DrawSprite(Sprites[NameToId["field"]]);
}

void Game::UpdatePlay()
{
    UpdatePositions();
    HandleCollisions();
}

void Game::DrawPlay(gfx::Renderer<>& renderer)
{
    for (ptrdiff_t i = NameToId["field"]; i >= 0; --i) {
        renderer.DrawSprite(Sprites[i]);
    }
}

void Game::UpdateWon() {}
void Game::DrawWon(gfx::Renderer<>& renderer)
{
    renderer.DrawSprite(Sprites[NameToId["field"]]);
}

void Game::UpdatePause() {}
void Game::DrawPause(gfx::Renderer<>& renderer)
{
    for (ptrdiff_t i = NameToId["field"]; i >= 0; --i) {
        renderer.DrawSprite(Sprites[i]);
    }
}

void Game::ResetPositions()
{
    auto& sizes = Entities.get<1>();

    int ball = NameToId["ball"];
    int paddle = NameToId["player1"];
    std::array<math::Vec2<float>, 3> starting_positions = {
        math::Vec2<float>{
            FieldBbox.Pos.x() + FieldBbox.Size.x() / 2.0f - sizes[ball].x() / 2.0f,
            FieldBbox.Pos.y() + FieldBbox.Size.y() / 2.0f - sizes[ball].y() / 2.0f
        },
        math::Vec2<float>{
            FieldBbox.Pos.x() + 1.0f,
            FieldBbox.Pos.y() + FieldBbox.Size.y() / 2.0f - sizes[paddle].y() / 2.0f
        },
        math::Vec2<float>{
            FieldBbox.Pos.x() + FieldBbox.Size.x() - sizes[paddle].x() - 1.0f,
            FieldBbox.Pos.y() + FieldBbox.Size.y() / 2.0f - sizes[paddle].y() / 2.0f
        }
    };

    auto& positions = Entities.get<0>();
    auto& velocities = Entities.get<2>();
    for (int i = 0; i < EntityCount; ++i) {
        velocities[i] = { 0.0f, 0.0f };
        positions[i] = starting_positions[i];
        Sprites[i].Bbox.Pos = starting_positions[i];
    }
}

math::Bbox Game::GetBbox(const char* name)
{
    int id = NameToId[name];
    return Sprites[id].Bbox;
}


math::Vec2<float> Game::GetVelocity(const char* name)
{
    int id = NameToId[name];
    return Entities.get<2>()[id];
}

void Game::SetVelocity(const char* name, math::Vec2<float> v)
{
    int id = NameToId[name];
    Entities.get<2>()[id] = v;
}

void Game::HandleInput(glfw::Window& window)
{
    if (window.GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (State == GameState::Pause || State == GameState::PlayerWon) State = GameState::Play;
        else if (State == GameState::Start) {
            StartGame();
        }
    }

    if (window.GetKey(GLFW_KEY_P) == GLFW_PRESS) {
        if (State == GameState::Play) State = GameState::Pause;
    }

    if (window.GetKey(GLFW_KEY_Q) == GLFW_PRESS) {
        QuitGame();
        return;
    }

    SetVelocity("player1", math::Vec2<float>(0.0f));
    if (window.GetKey(GLFW_KEY_S) == GLFW_PRESS) { SetVelocity("player1", { 0.0f, PaddleSpeed }); }
    if (window.GetKey(GLFW_KEY_W) == GLFW_PRESS) { SetVelocity("player1", { 0.0f, -PaddleSpeed }); }

    SetVelocity("player2", math::Vec2<float>(0.0f));
    if (window.GetKey(GLFW_KEY_DOWN) == GLFW_PRESS) { SetVelocity("player2", { 0.0f, PaddleSpeed }); }
    if (window.GetKey(GLFW_KEY_UP) == GLFW_PRESS) { SetVelocity("player2", { 0.0f, -PaddleSpeed }); }
}

void Game::UpdatePositions()
{
    auto& positions = Entities.get<0>();
    auto& sizes = Entities.get<1>();
    auto& velocities = Entities.get<2>();
    int ball_id = NameToId["ball"];
    for (size_t i = 0; i < positions.size(); ++i) {
        auto updated = positions[i] + velocities[i];
        const auto bbox = math::Bbox{ updated, sizes[i] };
        if (FieldBbox.Contains(bbox)) {
            positions[i] = updated;
            Sprites[i].Bbox.Pos = updated;
        } else if (ball_id == i) {
            if (bbox.Pos.y() <= FieldBbox.Pos.y() || bbox.Pos.y() + bbox.Size.y() >= FieldBbox.Pos.y() + FieldBbox.Size.y()) {
                velocities[i].y() *= -1.0f;
            } else {
                positions[i] = updated;
                Sprites[i].Bbox.Pos = updated;
            }
        }
    }
}

void Game::HandleCollisions()
{
    auto ball = GetBbox("ball");
    auto ball_velocity = GetVelocity("ball");
    auto player1 = GetBbox("player1");
    auto player2 = GetBbox("player2");

    if (ball.Pos.x() <= FieldBbox.Pos.x()) {
        Player2Score++;
        ResetPositions();
        return;
    }

    if (ball.Pos.x() + ball.Size.x() >= FieldBbox.Pos.x() + FieldBbox.Size.x()) {
        Player1Score++;
        ResetPositions();
        return;
    }

    auto ball_temp = ball;
    ball_temp.Pos = ball_temp.Pos + ball_velocity;

    if (ball_temp.Intersects(player1)) {
        auto direction = ball.Center() - player1.Center();
        float direction_length = std::sqrtf(direction.x() * direction.x() + direction.y() * direction.y());
        direction = (direction / direction_length) * BallSpeed;
        SetVelocity("ball", direction);
    }

    if (ball_temp.Intersects(player2)) {
        auto direction = ball.Center() - player2.Center();
        float direction_length = std::sqrtf(direction.x() * direction.x() + direction.y() * direction.y());
        direction = (direction / direction_length) * BallSpeed;
        SetVelocity("ball", direction);
    }
}

void Game::QuitGame()
{
    State = GameState::Quit;
    return;
}