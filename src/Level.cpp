#include "Game.hpp"
#include "Level.hpp"
#include "GameOver.hpp"
#include "PauseGame.hpp"
#include "Ball.hpp"
#include "Wall.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"
#include "ContactListener.hpp"

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

Level::Level(const std::shared_ptr<Context> &context) : m_entityMap(std::make_shared<std::multimap<BodyType, std::unique_ptr<Body>>>()),
                                                        m_context(context),
                                                        m_mouseJoint(nullptr),
                                                        m_targetPosition({0.f, 0.f}),
                                                        m_score(0),
                                                        m_isPaused(false),
                                                        m_tileSize(32.f),
                                                        m_windowSize(m_context->m_window->getDefaultView().getSize()),
                                                        m_contactListener(std::make_unique<ContactListener>(m_entityMap)),
                                                        m_levelUpTime(sf::Time::Zero),
                                                        m_isLevelUp(false),
                                                        m_levelCompleteText(),
                                                        m_levelUpWaitTime(sf::seconds(2))
{
    srand((unsigned int)time(nullptr));
    m_context->m_world->SetContactListener(m_contactListener.get());
}

Level::~Level()
{
}

void Level::Init()
{
    m_entityMap->emplace(std::make_pair(BodyType::Wall, std::make_unique<Wall>(m_context, sf::Vector2f{m_windowSize.x, m_tileSize}, sf::Vector2f{m_windowSize.x / 2, m_tileSize / 2})));
    m_entityMap->emplace(std::make_pair(BodyType::Wall, std::make_unique<Wall>(m_context, sf::Vector2f{m_tileSize, m_windowSize.y}, sf::Vector2f{m_tileSize / 2, (m_windowSize.y / 2 + m_tileSize)})));
    m_entityMap->emplace(std::make_pair(BodyType::Wall, std::make_unique<Wall>(m_context, sf::Vector2f{m_tileSize, m_windowSize.y}, sf::Vector2f{(m_windowSize.x - (m_tileSize / 2)), (m_windowSize.y / 2 + m_tileSize)})));
    m_entityMap->emplace(std::make_pair(BodyType::Wall, std::make_unique<Wall>(m_context, sf::Vector2f{m_windowSize.x, m_tileSize}, sf::Vector2f{m_windowSize.x / 2, (m_windowSize.y - m_tileSize / 2)})));

    auto position = sf::Vector2f(64.f * 2, 64.f * 2);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            m_entityMap->emplace(std::make_pair(BodyType::Brick, std::make_unique<Brick>(m_context, sf::Vector2f{(m_tileSize * 2), (m_tileSize / 2)}, position)));
            position.x += 64.f + 16.f;
        }
        position.x = 64.f * 2;
        position.y += 32.f;
    }

    auto ballRadius = 8.f;
    m_targetPosition = {(m_windowSize.x / 2), (m_windowSize.y - (1.5f * m_tileSize))};
    m_entityMap->emplace(std::make_pair(BodyType::Ball, std::make_unique<Ball>(m_context, ballRadius, sf::Vector2f{m_targetPosition.x, m_targetPosition.y - 16.f})));
    m_entityMap->emplace(std::make_pair(BodyType::Paddle, std::make_unique<Paddle>(m_context, sf::Vector2f{(m_tileSize * 3), (m_tileSize / 2)}, sf::Vector2f{m_targetPosition})));

    auto paddleIt = m_entityMap->find(BodyType::Paddle);
    auto wallIt = m_entityMap->find(BodyType::Wall);
    if ((paddleIt != m_entityMap->end()) && (wallIt != m_entityMap->end()))
    {
        m_mouseJoint = CreateMouseJoint(*paddleIt->second->GetB2Body(), *wallIt->second->GetB2Body());

        // Create a prismatic joint to restrict motion of paddle along y-axis.
        b2Vec2 worldAxis(1.0f, 0.0f);
        b2PrismaticJointDef jointDef;
        jointDef.collideConnected = true;
        jointDef.Initialize(wallIt->second->GetB2Body(), paddleIt->second->GetB2Body(), wallIt->second->GetB2Body()->GetWorldCenter(), worldAxis);
        auto joint = m_context->m_world->CreateJoint(&jointDef);
    }

    m_scoreText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_scoreText.setString("Score : " + std::to_string(m_score));
    m_scoreText.setFillColor(sf::Color(255, 128, 0));
    m_scoreText.setOutlineColor(sf::Color::Black);
    m_scoreText.setOutlineThickness(0.f);
    m_scoreText.setCharacterSize(15);

    m_levelCompleteText.setFont(m_context->m_assets->GetFont(MAIN_FONT));
    m_levelCompleteText.setString("Level Complete!");
    m_levelCompleteText.setFillColor(sf::Color(255, 128, 0));
    m_levelCompleteText.setOutlineColor(sf::Color::Black);
    m_levelCompleteText.setOutlineThickness(0.f);
    m_levelCompleteText.setCharacterSize(50);
    m_levelCompleteText.setOrigin(m_levelCompleteText.getLocalBounds().width / 2, m_levelCompleteText.getLocalBounds().height / 2);
    m_levelCompleteText.setPosition(m_windowSize.x / 2, m_windowSize.y / 2);
}

void Level::ProcessInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        if (m_targetPosition.x > 0.f)
        {
            m_targetPosition -= sf::Vector2f(15.f, 0.f);
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
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

void Level::Update(sf::Time deltaTime)
{
    if (!m_isPaused)
    {
        //if (m_ball->GetLinearVelocity().Length() > 10.f)
        //{
        //    m_ball->SetLinearDamping(10.f);
        //}
        //else
        //{
        //    m_ball->SetLinearDamping(0.f);
        //}

        m_mouseJoint->SetTarget({m_targetPosition.x / m_context->scale, m_targetPosition.y / m_context->scale});

        m_context->m_world->Step(deltaTime.asSeconds(), 8, 3);

        for (auto &entity : *m_entityMap)
        {
            entity.second->Update(deltaTime);
        }

        m_contactListener->DeleteCollidedBodies(m_context->m_world.get());

        if (m_entityMap->find(BodyType::Brick) == m_entityMap->end())
        {
            m_isLevelUp = true;
            this->Pause();
        }
    }
    if (m_isLevelUp)
    {
        m_levelUpTime += deltaTime;
        if (m_levelUpTime > m_levelUpWaitTime)
        {
            this->LoadNextLevel();

            // Now destory all the entities.
            for (auto &entity : *m_entityMap)
            {
                m_context->m_world->DestroyBody(entity.second.get()->GetB2Body());
            }
        }
    }
}

void Level::Draw()
{
    m_context->m_window->clear();

    for (auto &entity : *m_entityMap)
    {
        m_context->m_window->draw(*entity.second);
    }

    if(m_isLevelUp)
    {
        m_context->m_window->draw(m_levelCompleteText);
    }

    m_context->m_window->draw(m_scoreText);
    m_context->m_window->display();
}

void Level::Pause()
{
    m_isPaused = true;
}

void Level::Start()
{
    m_isPaused = false;
}

b2MouseJoint *Level::CreateMouseJoint(b2Body &bodyToMove, b2Body &groundBody)
{
    auto mouseJointDef = b2MouseJointDef();
    mouseJointDef.bodyA = &groundBody;
    mouseJointDef.bodyB = &bodyToMove;
    mouseJointDef.target = bodyToMove.GetPosition();
    mouseJointDef.collideConnected = true;
    mouseJointDef.maxForce = 1000.f;

    b2LinearStiffness(mouseJointDef.stiffness, mouseJointDef.damping, 2000.f, 1.f, &groundBody, &bodyToMove);

    auto moustJoint = static_cast<b2MouseJoint *>(m_context->m_world->CreateJoint(&mouseJointDef));
    return moustJoint;
}
