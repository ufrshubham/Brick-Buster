#include "Brick.hpp"
#include "Game.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include <SFML/Graphics/RectangleShape.hpp>

b2Body* Brick::CreateBrick(const sf::Vector2f& size, const sf::Vector2f position)
{
    // This defines the definitions of current brick. 
    // Drawable created above will be stored as userData in bodyDef.
    auto bodyDef = b2BodyDef();
    bodyDef.position = b2Vec2((position.x / m_context->scale), (position.y / m_context->scale));
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;

    // Define physical shape and properties of current brick.
    auto bodyShape = b2PolygonShape();
    bodyShape.SetAsBox((size.x / m_context->scale) / 2, (size.y / m_context->scale) / 2);
    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.f;
    fixtureDef.shape = &bodyShape;

    // Create brick and attached fixture to it.
    auto brickBody = m_context->m_world->CreateBody(&bodyDef);
    brickBody->CreateFixture(&fixtureDef);

    return brickBody;
}

Brick::Brick(std::shared_ptr<Context>& context, const sf::Vector2f& size, const sf::Vector2f position) :
    Body(context)
{
    m_body = CreateBrick(size, position);

    // This the drawable object for current brick.
    auto drawableBrick = new sf::RectangleShape(size);
    drawableBrick->setOrigin(drawableBrick->getSize().x / 2, drawableBrick->getSize().y / 2);
    drawableBrick->setPosition(position);
    drawableBrick->setFillColor(sf::Color::Green);

    m_shape = drawableBrick;
}

BodyType Brick::GetType() const
{
    return BodyType::Brick;
}
