#include "Body.hpp"
#include "Game.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Shape.hpp>

Body::Body(std::shared_ptr<Context> &context) : 
    m_body(nullptr),
    m_shape(nullptr),
    m_context(context)
{
}

Body::~Body()
{
    delete m_shape;
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*m_shape);
}

b2Body* Body::GetB2Body()
{
    return m_body;
}

void Body::Update(const sf::Time& deltaTime)
{
    switch (this->GetType())
    {
    case BodyType::Ball:
    case BodyType::Paddle:
        m_shape->setPosition(m_body->GetPosition().x * m_context->scale, m_body->GetPosition().y * m_context->scale);
        m_shape->setRotation(m_body->GetAngle() * 180 / b2_pi);
        break;
    case BodyType::Wall:
        break;
    default:
        break;
    }
}

