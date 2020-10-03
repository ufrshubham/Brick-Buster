#include "GamePlay.hpp"
#include "GameOver.hpp"
#include "PauseGame.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include <stdlib.h>
#include <time.h>

#include <iostream>

GamePlay::GamePlay(std::shared_ptr<Context>& context) :
    m_context(context),
    m_walls(),
    m_ball(nullptr),
    m_paddle(nullptr),
    m_paddleVelocity(0.0),
    m_score(0),
    m_elapsedTime(sf::Time::Zero),
    m_isPaused(false)
{
    srand(time(nullptr));
}

GamePlay::~GamePlay()
{
    for (auto& wall : m_walls)
    {
        delete reinterpret_cast<sf::Shape*>(wall->GetUserData());
    }
    delete reinterpret_cast<sf::Shape*>(m_ball->GetUserData());
    delete reinterpret_cast<sf::Shape*>(m_paddle->GetUserData());
}

void GamePlay::Init()
{
    auto windowSize = m_context->m_window->getDefaultView().getSize();
    auto tileSize = 32.f;
    m_walls.at(0) = CreateWall({ windowSize.x, tileSize }, { windowSize.x / 2 , tileSize / 2 });
    m_walls.at(1) = CreateWall({ tileSize, windowSize.y }, { tileSize / 2 , (windowSize .y / 2 + tileSize) });
    m_walls.at(2) = CreateWall({ tileSize, windowSize.y }, { (windowSize.x - (tileSize / 2)) , (windowSize.y / 2 + tileSize) });
    m_walls.at(3) = CreateWall({ windowSize.x, tileSize }, { windowSize.x / 2  , (windowSize.y - tileSize / 2) });

    auto ballRadius = 8.f;
    m_ball = CreateBall(ballRadius, { windowSize.x / 2, windowSize.y / 2});

    m_paddle = CreatePaddle({ (tileSize * 2), (tileSize / 2) }, { (windowSize.x / 2), (windowSize.y - (1.5f * tileSize)) });

    m_scoreText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_scoreText.setString("Score : " + std::to_string(m_score));
    m_scoreText.setFillColor(sf::Color(255, 128, 0));
    m_scoreText.setOutlineColor(sf::Color::Black);
    m_scoreText.setOutlineThickness(0.f);
    m_scoreText.setCharacterSize(15.f);
}

void GamePlay::ProcessInput()
{
    sf::Event event;
    while (m_context->m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_context->m_window->close();
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                break;
            case sf::Keyboard::Down:
                break;
            case sf::Keyboard::Left:
                m_paddleVelocity = std::clamp<float>(m_paddleVelocity - 500, -1000, 1000);
                break;
            case sf::Keyboard::Right:
                m_paddleVelocity = std::clamp<float>(m_paddleVelocity + 500, -1000, 1000);
                break;
            case sf::Keyboard::Escape:
                m_context->m_states->Add(std::make_unique<PauseGame>(m_context));
                break;

            default:
                break;
            }
        }
    }
}

void GamePlay::Update(sf::Time deltaTime)
{
    if(!m_isPaused)
    {
        m_paddle->SetLinearVelocity({ m_paddleVelocity / m_context->scale, 0.f });

        m_context->m_world->Step(deltaTime.asSeconds(), 8, 3);

        auto ball = reinterpret_cast<sf::Shape*>(m_ball->GetUserData());
        ball->setPosition(m_ball->GetPosition().x * m_context->scale, m_ball->GetPosition().y * m_context->scale);
        ball->setRotation(m_ball->GetAngle() * 180 / b2_pi);

        auto paddle = reinterpret_cast<sf::Shape*>(m_paddle->GetUserData());
        paddle->setPosition(m_paddle->GetPosition().x * m_context->scale, m_paddle->GetPosition().y * m_context->scale);
        
        if (m_paddleVelocity > 0.0)
        {
            m_paddleVelocity = std::clamp<float>(m_paddleVelocity - 500, 0.0, 200);
        }
        else
        {
            m_paddleVelocity = std::clamp<float>(m_paddleVelocity + 500, -200, 0.0);
        }
    }
}

void GamePlay::Draw()
{
    m_context->m_window->clear();

    for (auto &wall : m_walls)
    {
        m_context->m_window->draw(*reinterpret_cast<sf::RectangleShape*>(wall->GetUserData()));
    }

    m_context->m_window->draw(*reinterpret_cast<sf::Shape*>(m_ball->GetUserData()));
    m_context->m_window->draw(*reinterpret_cast<sf::Shape*>(m_paddle->GetUserData()));
    m_context->m_window->draw(m_scoreText);
    m_context->m_window->display();
}

void GamePlay::Pause()
{
    m_isPaused = true;
}

void GamePlay::Start()
{
    m_isPaused = false;
}

b2Body* GamePlay::CreateWall(const sf::Vector2f& size, const sf::Vector2f position)
{
    // This the drawable object for current wall.
    // Current state has to make sure to delete this drawable.
    auto drawableWall = new sf::RectangleShape(size);
    drawableWall->setOrigin(drawableWall->getSize().x / 2, drawableWall->getSize().y / 2);
    drawableWall->setPosition(position);
    drawableWall->setFillColor(sf::Color::Green);

    // This defines the definitions of current wall. 
    // Drawable created above will be stored as userData in bodyDef.
    auto bodyDef = b2BodyDef();
    bodyDef.position = b2Vec2((position.x / m_context->scale), (position.y / m_context->scale));
    bodyDef.type = b2_staticBody;
    bodyDef.userData = drawableWall;

    // Define physical shape and properties of current wall.
    auto bodyShape = b2PolygonShape();
    bodyShape.SetAsBox((size.x / m_context->scale) / 2, (size.y / m_context->scale) / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &bodyShape;

    // Create wall and attached fixture to it.
    auto wallBody = m_context->m_world->CreateBody(&bodyDef);
    wallBody->CreateFixture(&fixtureDef);

    return wallBody;
}

b2Body* GamePlay::CreateBall(const float& radius, const sf::Vector2f& position)
{
    auto drawableBall = new sf::CircleShape(radius);
    drawableBall->setFillColor(sf::Color::Red);
    drawableBall->setOrigin(drawableBall->getRadius(), drawableBall->getRadius());
    drawableBall->setPosition(position);

    b2BodyDef bodyDefBall;
    bodyDefBall.position = b2Vec2(position.x / m_context->scale, position.y / m_context->scale);
    bodyDefBall.type = b2_dynamicBody;
    bodyDefBall.userData = drawableBall;

    auto ballShape = b2CircleShape();
    ballShape.m_radius = radius / m_context->scale;
    b2FixtureDef ballFixture;
    ballFixture.density = 1.f;
    ballFixture.friction = 0.f;
    ballFixture.restitution = 1.f;
    ballFixture.shape = &ballShape;

    auto ballBody = m_context->m_world->CreateBody(&bodyDefBall);
    ballBody->CreateFixture(&ballFixture);

    ballBody->ApplyLinearImpulse({100.0f / m_context->scale, -120.0f / m_context->scale }, bodyDefBall.position, true);

    return ballBody;
}

b2Body* GamePlay::CreatePaddle(const sf::Vector2f& size, const sf::Vector2f position)
{
    // This the drawable object for current wall.
    // Current state has to make sure to delete this drawable.
    auto drawableWall = new sf::RectangleShape(size);
    drawableWall->setOrigin(drawableWall->getSize().x / 2, drawableWall->getSize().y / 2);
    drawableWall->setPosition(position);
    drawableWall->setFillColor(sf::Color::Green);

    // This defines the definitions of current wall. 
    // Drawable created above will be stored as userData in bodyDef.
    auto bodyDef = b2BodyDef();
    bodyDef.position = b2Vec2((position.x / m_context->scale), (position.y / m_context->scale));
    bodyDef.type = b2_kinematicBody;
    bodyDef.userData = drawableWall;

    // Define physical shape and properties of current wall.
    auto paddleShape = b2PolygonShape();
    paddleShape.SetAsBox((size.x / m_context->scale) / 2, (size.y / m_context->scale) / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.1f;
    fixtureDef.shape = &paddleShape;

    // Create wall and attached fixture to it.
    auto wallBody = m_context->m_world->CreateBody(&bodyDef);
    wallBody->CreateFixture(&fixtureDef);

    return wallBody;
}

