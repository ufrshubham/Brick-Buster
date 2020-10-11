#pragma once

#include "Ball.hpp"

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_body.h>

#include <vector>
#include <map>

class ContactListener : public b2ContactListener
{
private:
	std::shared_ptr<std::multimap<BodyType, std::unique_ptr<Body>>> m_entityMap;
	std::vector<b2Body*> m_bodiesToBeDeleted;

public:
	ContactListener(std::shared_ptr<std::multimap<BodyType, std::unique_ptr<Body>>>& entityMap);
	virtual ~ContactListener();

	virtual void EndContact(b2Contact* contact) final;

	void DeleteCollidedBodies(b2World* world);
};