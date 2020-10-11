#include "Paddle.hpp"
#include "Game.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_prismatic_joint.h>

#include <SFML/Graphics/RectangleShape.hpp>

b2Body* Paddle::CreatePaddle(const sf::Vector2f& size, const sf::Vector2f position)
{
    // This defines the definitions of paddle. 
    // Drawable created above will be stored as userData in bodyDef.
    auto bodyDef = b2BodyDef();
    bodyDef.position = b2Vec2((position.x / m_context->scale), (position.y / m_context->scale));
    bodyDef.type = b2_dynamicBody;
    bodyDef.userData = this;

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

    return paddleBody;
}

Paddle::Paddle(std::shared_ptr<Context>& context, const sf::Vector2f& size, const sf::Vector2f position) :
    Body(context)
{
    m_body = CreatePaddle(size, position);

    // This the drawable object for this paddle.
    auto drawablePaddle = new sf::RectangleShape(size);
    drawablePaddle->setOrigin(drawablePaddle->getSize().x / 2, drawablePaddle->getSize().y / 2);
    drawablePaddle->setPosition(position);
    drawablePaddle->setFillColor(sf::Color::Blue);

    m_shape = drawablePaddle;
}

BodyType Paddle::GetType() const
{
    return BodyType::Paddle;
}
