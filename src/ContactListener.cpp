#include "ContactListener.hpp"

#include <SFML/Graphics/Shape.hpp>

#include <box2d/b2_contact.h>
#include <box2d/b2_world.h>

ContactListener::ContactListener() : 
    m_ball(nullptr),
    m_paddle(nullptr),
    m_bodiesToBeDeleted()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::SetBall(const b2Body* ball)
{
    m_ball = ball;
}

void ContactListener::SetPaddle(const b2Body* paddle)
{
    m_paddle = paddle;
}

void ContactListener::EndContact(b2Contact* contact)
{
    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    if((bodyA != m_paddle) && (bodyB != m_paddle))
    {
        if(bodyA == m_ball)
        {
            m_bodiesToBeDeleted.push_back(bodyB);
        }
        else if(bodyB == m_ball)
        {
            m_bodiesToBeDeleted.push_back(bodyA);
        }
    }
}

void ContactListener::DeleteCollidedBodies(b2World* world, std::vector<b2Body*>& bricks)
{
    for(auto& body : m_bodiesToBeDeleted)
    {
        const auto& it = std::find(bricks.begin(), bricks.end(), body);
        if(it!=bricks.end())
        {
            bricks.erase(it);
            world->DestroyBody(body);
        }
    }
    m_bodiesToBeDeleted.clear();
}
