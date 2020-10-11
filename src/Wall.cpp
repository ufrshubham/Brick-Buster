#include "Wall.hpp"
#include "Game.hpp"

#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_prismatic_joint.h>

#include <SFML/Graphics/RectangleShape.hpp>

b2Body* Wall::CreateWall(const sf::Vector2f& size, const sf::Vector2f position)
{
    // This defines the definitions of current wall. 
    // Drawable created above will be stored as userData in bodyDef.
    auto bodyDef = b2BodyDef();
    bodyDef.position = b2Vec2((position.x / m_context->scale), (position.y / m_context->scale));
    bodyDef.type = b2_staticBody;
    bodyDef.userData = this;

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

Wall::Wall(std::shared_ptr<Context>& context, const sf::Vector2f& size, const sf::Vector2f position) :
    Body(context)

{
    m_body = CreateWall(size, position);

    // This the drawable object for current wall.
    auto drawableWall = new sf::RectangleShape(size);
    drawableWall->setOrigin(drawableWall->getSize().x / 2, drawableWall->getSize().y / 2);
    drawableWall->setPosition(position);
    drawableWall->setFillColor(sf::Color::Green);

    m_shape = drawableWall;
}

BodyType Wall::GetType() const
{
    return BodyType::Wall;
}
