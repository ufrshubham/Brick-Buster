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

GamePlay::GamePlay(std::shared_ptr<Context>& context) :
    m_context(context),
    m_walls(),
    m_ball(nullptr),
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
        delete reinterpret_cast<sf::RectangleShape*>(wall->GetUserData());
    }
    delete reinterpret_cast<sf::RectangleShape*>(m_ball->GetUserData());
}

void GamePlay::Init()
{
    auto windowSize = m_context->m_window->getDefaultView().getSize();
    auto tileSize = 32.f;
    m_walls.at(0) = CreateWall({ windowSize.x, tileSize }, { windowSize.x / 2 , tileSize / 2 });
    m_walls.at(1) = CreateWall({ windowSize.x, tileSize }, { windowSize .x / 2 , (windowSize .y - tileSize / 2) });

    // Ball
    auto ball = new sf::CircleShape(8.f);
    ball->setOrigin(ball->getRadius(), ball->getRadius());
    ball->setPosition(1280.f/2, 720.f/2);

    b2BodyDef bodyDefBall;
    bodyDefBall.position = b2Vec2(1280.f / (2 * m_context->scale), 720.f / (2 * m_context->scale));
    bodyDefBall.type = b2_dynamicBody;
    bodyDefBall.userData = ball;
    m_ball = m_context->m_world->CreateBody(&bodyDefBall);

    b2PolygonShape ballShape;
    ballShape.SetAsBox((16.f / 2) / m_context->scale, (16.f / 2) / m_context->scale);
    b2FixtureDef ballFixture;
    ballFixture.restitution = 0.5;
    ballFixture.density = 1.f;
    ballFixture.friction = 0.7f;
    ballFixture.shape = &ballShape;
    m_ball->CreateFixture(&ballFixture);

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
                m_ball->ApplyForceToCenter(b2Vec2(0.f, -100.f), true);
                break;
            case sf::Keyboard::Down:
                m_ball->ApplyForceToCenter(b2Vec2(0.f, 100.f), true);
                break;
            case sf::Keyboard::Left:
                m_ball->ApplyForceToCenter(b2Vec2(-100.f, 0.f), true);
                break;
            case sf::Keyboard::Right:
                m_ball->ApplyForceToCenter(b2Vec2(100.f, 0.f), true);
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
        auto ball = reinterpret_cast<sf::CircleShape*>(m_ball->GetUserData());
        ball->setPosition(m_ball->GetPosition().x * m_context->scale, m_ball->GetPosition().y * m_context->scale);
        ball->setRotation(m_ball->GetAngle() * 180 / b2_pi);
        m_context->m_world->Step(1.f / 60.f, 8, 3);
    }
}

void GamePlay::Draw()
{
    m_context->m_window->clear();

    for (auto &wall : m_walls)
    {
        m_context->m_window->draw(*reinterpret_cast<sf::RectangleShape*>(wall->GetUserData()));
    }
    m_context->m_window->draw(*reinterpret_cast<sf::CircleShape*>(m_ball->GetUserData()));
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
    auto shapeTop = b2PolygonShape();
    shapeTop.SetAsBox((size.x / m_context->scale) / 2, (size.y / m_context->scale) / 2);
    b2FixtureDef fixtureDefTop;
    fixtureDefTop.density = 0.f;
    fixtureDefTop.shape = &shapeTop;

    // Create wall and attached fixture to it.
    auto wallBody = m_context->m_world->CreateBody(&bodyDef);
    wallBody->CreateFixture(&fixtureDefTop);

    return wallBody;
}

