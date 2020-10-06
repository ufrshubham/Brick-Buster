#pragma once

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_body.h>

#include <vector>

class ContactListener : public b2ContactListener
{
private:
	const b2Body* m_ball;
	const b2Body* m_paddle;
	std::vector<b2Body*> m_bodiesToBeDeleted;

public:
	ContactListener();
	virtual ~ContactListener();

	void SetBall(const b2Body* ball);
	void SetPaddle(const b2Body* paddle);

	virtual void EndContact(b2Contact* contact) final;

	void DeleteCollidedBodies(b2World* world, std::vector<b2Body*>& bricks);
};