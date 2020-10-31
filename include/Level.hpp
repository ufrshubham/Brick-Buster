#pragma once

#include <memory>
#include <array>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_mouse_joint.h>

#include "State.hpp"

struct Context;
class Body;
class ContactListener;
enum class BodyType;

class Level : public Engine::State
{
protected:
    std::shared_ptr<Context> m_context;

private:
    std::shared_ptr<std::multimap<BodyType, std::unique_ptr<Body>>> m_entityMap;

    b2MouseJoint* m_mouseJoint;
    sf::Vector2f m_targetPosition;

    sf::Text m_scoreText;
    int m_score;

    bool m_isPaused;
    
    const float m_tileSize;
    const sf::Vector2f m_windowSize;

    std::unique_ptr<ContactListener> m_contactListener;

    sf::Time m_levelUpTime;
    bool m_isLevelUp;

    sf::Text m_levelCompleteText;
    const sf::Time m_levelUpWaitTime;

public:
    Level(const std::shared_ptr<Context> &context);
    virtual ~Level();

    void Init() override;
    void ProcessInput() override;
    void Update(sf::Time deltaTime) override;
    void Draw() override;
    void Pause() override;
    void Start() override;

    virtual void LoadNextLevel() const = 0;

private:

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