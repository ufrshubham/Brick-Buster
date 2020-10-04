#pragma once

#include <memory>
#include <array>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_mouse_joint.h>

#include "Game.hpp"
#include "State.hpp"

class GamePlay : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;
    std::array<b2Body*, 4> m_walls;

    b2Body* m_ball;
    b2Body* m_paddle;

    b2MouseJoint* m_mouseJoint;
    sf::Vector2f m_targetPosition;

    sf::Text m_scoreText;
    int m_score;

    bool m_isPaused;
    
    const float m_tileSize;
    const sf::Vector2f m_windowSize;

public:
    GamePlay(std::shared_ptr<Context> &context);
    ~GamePlay();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;
    void Pause() override;
    void Start() override;

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

    /**
     * \brief This method creates a new dynamic b2Body. It internally attaches a drawable object
     * as userData in body definition. Use b2Body::GetUserData() to get this drawable.
     * 
     * \param radius Radius of circle in pixel space.
     * \param position Position of the ball in pixel space.
     * \return A new circular b2Body.
     */
    b2Body* CreateBall(const float& radius, const sf::Vector2f& position);

    /**
     * \brief This method creates a new dynamic b2Body. It internally attaches a drawable object
     * as userData in body definition. Use b2Body::GetUserData() to get this drawable.
     * 
     * \param size Size of the paddle in pixel space.
     * \param position Position of the paddle in pixel space.
     * \return A new rectangular b2Body.
     */
    b2Body* CreatePaddle(const sf::Vector2f& size, const sf::Vector2f position);

    /**
     * \brief This method creates a new b2MouseJoint between given two bodies.
     * Initially, the target position of this joint will be set a bodyToMove's current position.
     * To make the body move, change the target position of this joint using b2MouseJoint::SetTargetPosition().
     * 
     * \param bodyToMove Body to be used as bodyB in this joint.
     * \param groundBody Body to be used as bodyA in this joint.
     * \return A new b2MouseJoint.
     */
    b2MouseJoint* CreateMouseJoint(b2Body& bodyToMove, b2Body& groundBody);
};