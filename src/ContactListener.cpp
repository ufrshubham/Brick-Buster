#include "ContactListener.hpp"

#include <SFML/Graphics/Shape.hpp>

#include <box2d/b2_contact.h>
#include <box2d/b2_world.h>

ContactListener::ContactListener(std::shared_ptr<std::multimap<BodyType, std::unique_ptr<Body>>>& entityMap) :
    m_entityMap(entityMap),
    m_bodiesToBeDeleted()
{
}

ContactListener::~ContactListener()
{
}

void ContactListener::EndContact(b2Contact* contact)
{
    auto b2BodyA = contact->GetFixtureA()->GetBody();
    auto b2BodyB = contact->GetFixtureB()->GetBody();

    auto bodyA = reinterpret_cast<Body*>(b2BodyA->GetUserData());
    auto bodyB = reinterpret_cast<Body*>(b2BodyB->GetUserData());

    if (bodyA && bodyB)
    {
        auto bodyAType = bodyA->GetType();
        auto bodyBType = bodyB->GetType();

        if ((bodyAType != BodyType::Wall) && (bodyBType != BodyType::Wall))
        {
            if ((bodyAType == BodyType::Ball) || (bodyBType == BodyType::Brick))
            {
                m_bodiesToBeDeleted.push_back(b2BodyB);
            }
            else if ((bodyBType == BodyType::Ball) || (bodyAType == BodyType::Brick))
            {
                m_bodiesToBeDeleted.push_back(b2BodyA);
            }
        }
    }
}

void ContactListener::DeleteCollidedBodies(b2World* world)
{
    for(auto& body : m_bodiesToBeDeleted)
    {
        auto range = m_entityMap->equal_range(BodyType::Brick);

        for (auto it = range.first; it != range.second; ++it)
        {
            if (it->second->GetB2Body() == body)
            {
                m_entityMap->erase(it);
                world->DestroyBody(body);
                break;
            }
        }
    }
    m_bodiesToBeDeleted.clear();
}
