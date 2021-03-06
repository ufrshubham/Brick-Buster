#pragma once

#include "Body.hpp"

class Paddle : public Body
{
private:
    /**
     * \brief This method creates a new dynamic b2Body. It internally attaches a drawable object
     * as userData in body definition. Use b2Body::GetUserData() to get this drawable.
     *
     * \param size Size of the paddle in pixel space.
     * \param position Position of the paddle in pixel space.
     * \return A new rectangular b2Body.
     */
    b2Body* CreatePaddle(const sf::Vector2f& size, const sf::Vector2f position);
public:
    Paddle(std::shared_ptr<Context>& context, const sf::Vector2f& size, const sf::Vector2f position);

    virtual BodyType GetType() const override;
};
