#include "GamePlay.hpp"
#include "GameOver.hpp"
#include "PauseGame.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_prismatic_joint.h>

#include <stdlib.h>
#include <time.h>

#include <iostream>

GamePlay::GamePlay(std::shared_ptr<Context>& context) :
    m_context(context),
    m_walls(),
    m_bricks(21),
    m_ball(nullptr),
    m_paddle(nullptr),
    m_mouseJoint(nullptr),
    m_targetPosition({0.f, 0.f}),
    m_score(0),
    m_isPaused(false),
    m_tileSize(32.f),
    m_windowSize(m_context->m_window->getDefaultView().getSize()),
    m_contactListener(std::make_unique<ContactListener>())
{
    srand(time(nullptr));
    m_context->m_world->SetContactListener(m_contactListener.get());
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
    m_walls.at(0) = CreateWall({ m_windowSize.x, m_tileSize }, { m_windowSize.x / 2 , m_tileSize / 2 });
    m_walls.at(1) = CreateWall({ m_tileSize, m_windowSize.y }, { m_tileSize / 2 , (m_windowSize.y / 2 + m_tileSize) });
    m_walls.at(2) = CreateWall({ m_tileSize, m_windowSize.y }, { (m_windowSize.x - (m_tileSize / 2)) , (m_windowSize.y / 2 + m_tileSize) });
    m_walls.at(3) = CreateWall({ m_windowSize.x, m_tileSize }, { m_windowSize.x / 2  , (m_windowSize.y - m_tileSize / 2) });

    auto position = sf::Vector2f(64.f * 2, 64.f * 2);
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 7; ++j)
        {
            m_bricks[i * 7 + j] = CreateWall({ (m_tileSize * 2), (m_tileSize / 2) }, position);
            position.x += 64.f + 16.f;
        }
        position.x = 64.f * 2;
        position.y += 32.f;
    }

    auto ballRadius = 8.f;
    m_targetPosition = { (m_windowSize.x / 2), (m_windowSize.y - (1.5f * m_tileSize)) };

    m_ball = CreateBall(ballRadius, {m_targetPosition.x, m_targetPosition.y - 16.f});
    m_paddle = CreatePaddle({ (m_tileSize * 3), (m_tileSize / 2) }, m_targetPosition);

    m_contactListener->SetBall(m_ball);
    m_contactListener->SetPaddle(m_paddle);

    m_mouseJoint = CreateMouseJoint(*m_paddle, *m_walls.at(0));

    m_scoreText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_scoreText.setString("Score : " + std::to_string(m_score));
    m_scoreText.setFillColor(sf::Color(255, 128, 0));
    m_scoreText.setOutlineColor(sf::Color::Black);
    m_scoreText.setOutlineThickness(0.f);
    m_scoreText.setCharacterSize(15);
}

void GamePlay::ProcessInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (m_targetPosition.x > 0.f)
        {
            m_targetPosition -= sf::Vector2f(15.f, 0.f);
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        if (m_targetPosition.x < m_windowSize.x)
        {
            m_targetPosition += sf::Vector2f(15.f, 0.f);
        }
    }

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
        if (m_ball->GetLinearVelocity().Length() > 10.f)
        {
            m_ball->SetLinearDamping(10.f);
        }
        else
        {
            m_ball->SetLinearDamping(0.f);
        }

        m_mouseJoint->SetTarget({ m_targetPosition.x / m_context->scale, m_targetPosition.y / m_context->scale });

        m_context->m_world->Step(deltaTime.asSeconds(), 8, 3);

        auto ball = reinterpret_cast<sf::Shape*>(m_ball->GetUserData());
        ball->setPosition(m_ball->GetPosition().x * m_context->scale, m_ball->GetPosition().y * m_context->scale);
        ball->setRotation(m_ball->GetAngle() * 180 / b2_pi);

        auto paddle = reinterpret_cast<sf::Shape*>(m_paddle->GetUserData());
        paddle->setPosition(m_paddle->GetPosition().x * m_context->scale, m_paddle->GetPosition().y * m_context->scale);

        m_contactListener->DeleteCollidedBodies(m_context->m_world.get(), m_bricks);
    }
}

void GamePlay::Draw()
{
    m_context->m_window->clear();

    for (auto &wall : m_walls)
    {
        m_context->m_window->draw(*reinterpret_cast<sf::Shape*>(wall->GetUserData()));
    }

    for(auto &brick : m_bricks)
    {
        if(brick)
        {
            m_context->m_window->draw(*reinterpret_cast<sf::Shape*>(brick->GetUserData()));
        }
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

    ballBody->ApplyLinearImpulse({-100.0f / m_context->scale, -100.0f / m_context->scale }, bodyDefBall.position, true);

    return ballBody;
}

b2Body* GamePlay::CreatePaddle(const sf::Vector2f& size, const sf::Vector2f position)
{
    // This the drawable object for this paddle.
    // Current state has to make sure to delete this drawable.
    auto drawablePaddle = new sf::RectangleShape(size);
    drawablePaddle->setOrigin(drawablePaddle->getSize().x / 2, drawablePaddle->getSize().y / 2);
    drawablePaddle->setPosition(position);
    drawablePaddle->setFillColor(sf::Color::Blue);

    // This defines the definitions of paddle. 
    // Drawable created above will be stored as userData in bodyDef.
    auto bodyDef = b2BodyDef();
    bodyDef.position = b2Vec2((position.x / m_context->scale), (position.y / m_context->scale));
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = drawablePaddle;

    // Define physical shape and properties of paddle.
    auto paddleShape = b2PolygonShape();
    paddleShape.SetAsBox((size.x / m_context->scale) / 2, (size.y / m_context->scale) / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.2f;
    fixtureDef.restitution = 0.1f;
    fixtureDef.friction = 0.f;
    fixtureDef.shape = &paddleShape;

    // Create paddle and attached fixture to it.
    auto paddleBody = m_context->m_world->CreateBody(&bodyDef);
    paddleBody->CreateFixture(&fixtureDef);
    paddleBody->SetLinearDamping(1);

    // Create a prismatic joint to restrict motion of paddle along y-axis.
    b2Vec2 worldAxis(1.0f, 0.0f);
    b2PrismaticJointDef jointDef;
    jointDef.collideConnected = true;
    jointDef.Initialize(m_walls.at(0), paddleBody, m_walls.at(0)->GetWorldCenter(), worldAxis);
    auto joint = m_context->m_world->CreateJoint(&jointDef);

    return paddleBody;
}

b2MouseJoint* GamePlay::CreateMouseJoint(b2Body& bodyToMove, b2Body& groundBody)
{
    auto mouseJointDef = b2MouseJointDef();
    mouseJointDef.bodyA = &groundBody;
    mouseJointDef.bodyB = &bodyToMove;
    mouseJointDef.target = bodyToMove.GetPosition();
    mouseJointDef.collideConnected = true;
    mouseJointDef.dampingRatio = 1.f;
    mouseJointDef.frequencyHz = 2000.f;
    mouseJointDef.maxForce = 1000.f;
    auto moustJoint = static_cast<b2MouseJoint*>(m_context->m_world->CreateJoint(&mouseJointDef));
    return moustJoint;
}

