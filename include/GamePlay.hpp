#pragma once

#include <memory>
#include <array>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <box2d/b2_body.h>

#include "Game.hpp"
#include "State.hpp"

class GamePlay : public Engine::State
{
private:
    std::shared_ptr<Context> m_context;
    std::array<b2Body*, 2> m_walls;
    b2Body* m_ball;

    sf::Text m_scoreText;
    int m_score;

    sf::Time m_elapsedTime;
    bool m_isPaused;

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
     * \brief This method creates a new b2Body. It internally attaches a drawable object
     * as userData in body definition. Use b2Body::GetUserData() to get this drawable.
     * 
     * \param size Size of the wall in pixel space.
     * \param position Position of the wall in pixel space.
     * \return A new b2Body.
     */
    b2Body* CreateWall(const sf::Vector2f& size, const sf::Vector2f position);
};