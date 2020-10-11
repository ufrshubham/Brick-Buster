#pragma once

#include "Body.hpp"

struct Context;

class Ball : public Body
{
private:
    /**
     * \brief This method creates a new dynamic b2Body. It internally attaches a drawable object
     * as userData in body definition. Use b2Body::GetUserData() to get this drawable.
     *
     * \param radius Radius of circle in pixel space.
     * \param position Position of the ball in pixel space.
     * \return A new circular b2Body.
     */
    b2Body* CreateBall(const float& radius, const sf::Vector2f& position);
public:
    Ball(std::shared_ptr<Context>& context, const  float& radius, const sf::Vector2f position);

    virtual BodyType GetType() const override;
};
