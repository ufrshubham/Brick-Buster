#include "Game.hpp"
#include "Level2.hpp"
#include "GameOver.hpp"

Level2::Level2(const std::shared_ptr<Context>& context) :
    Level(context)
{
}

Level2::~Level2()
{
}

void Level2::LoadNextLevel() const
{
    // Todo: Load-up next level.
    m_context->m_states->Add(std::make_unique<GameOver>(m_context));
}
