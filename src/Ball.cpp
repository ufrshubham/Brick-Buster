#include "Ball.hpp"
#include "Game.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_prismatic_joint.h>

#include <SFML/Graphics/CircleShape.hpp>

b2Body* Ball::CreateBall(const float& radius, const sf::Vector2f& position)
{
    b2BodyDef bodyDefBall;
    bodyDefBall.position = b2Vec2(position.x / m_context->scale, position.y / m_context->scale);
    bodyDefBall.type = b2_dynamicBody;
    bodyDefBall.userData = this;

    auto ballShape = b2CircleShape();
    ballShape.m_radius = radius / m_context->scale;
    b2FixtureDef ballFixture;
    ballFixture.density = 1.f;
    ballFixture.friction = 0.f;
    ballFixture.restitution = 1.f;
    ballFixture.shape = &ballShape;

    auto ballBody = m_context->m_world->CreateBody(&bodyDefBall);
    ballBody->CreateFixture(&ballFixture);

    ballBody->ApplyLinearImpulse({ -100.0f / m_context->scale, -100.0f / m_context->scale }, bodyDefBall.position, true);

    return ballBody;
}

Ball::Ball(std::shared_ptr<Context>& context, const float& radius, const sf::Vector2f position) :
    Body(context)
{
    m_body = CreateBall(radius, position);

    auto drawableBall = new sf::CircleShape(radius);
    drawableBall->setFillColor(sf::Color::Red);
    drawableBall->setOrigin(drawableBall->getRadius(), drawableBall->getRadius());
    drawableBall->setPosition(position);

    m_shape = drawableBall;
}

BodyType Ball::GetType() const
{
    return BodyType::Ball;
}
