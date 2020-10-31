#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_math.h>

#include "AssetMan.hpp"
#include "StateMan.hpp"

#include "ContactListener.hpp"

enum AssetID
{
    MAIN_FONT = 0,
};

struct Context
{
    float scale = 30.f;
    std::unique_ptr<Engine::AssetMan> m_assets;
    std::unique_ptr<Engine::StateMan> m_states;
    std::unique_ptr<sf::RenderWindow> m_window;
    std::unique_ptr<b2World> m_world;
    std::unique_ptr<ContactListener> m_contactListener;

    Context()
    {
        m_assets = std::make_unique<Engine::AssetMan>();
        m_states = std::make_unique<Engine::StateMan>();
        m_window = std::make_unique<sf::RenderWindow>();
        m_world = std::make_unique<b2World>(b2Vec2(0.f, 0.f));
    }
};

class Game
{
private:
    std::shared_ptr<Context> m_context;
    const sf::Time TIME_PER_FRAME = sf::seconds(1.f/60.f);

public:
    Game();
    ~Game();

    void Run();
};