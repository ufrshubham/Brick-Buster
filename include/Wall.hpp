#pragma once

#include "Body.hpp"

class Wall : public Body
{
private:
    /**
     * \brief This method creates a new static b2Body. It internally attaches a drawable object
     * as userData in body definition. Use b2Body::GetUserData() to get this drawable.
     * 
     * \param size Size of the wall in pixel space.
     * \param position Position of the wall in pixel space.
     * \return A new rectangular b2Body.
     */
    b2Body* CreateWall(const sf::Vector2f& size, const sf::Vector2f position);
public:
    Wall(std::shared_ptr<Context>& context, const sf::Vector2f& size, const sf::Vector2f position);

    virtual BodyType GetType() const override;
};
