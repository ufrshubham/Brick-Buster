#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Shape.hpp>

#include <box2d/b2_body.h>

#include <memory>

struct Context;

enum class BodyType
{
    Ball,
    Paddle,
    Wall,
    Brick
};

class Body : public sf::Drawable
{
protected:
    b2Body* m_body;
    sf::Shape* m_shape;

    std::shared_ptr<Context> m_context;

public:
    Body(std::shared_ptr<Context> &context);
    virtual ~Body();

    virtual BodyType GetType() const = 0;
    b2Body* GetB2Body();

    void Update(const sf::Time& deltaTime);

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
