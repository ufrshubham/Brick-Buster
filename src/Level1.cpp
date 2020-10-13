#include "Level1.hpp"
#include "GameOver.hpp"
#include "Game.hpp"

Level1::Level1(const std::shared_ptr<Context>& context) :
    Level(context)
{
}

Level1::~Level1()
{
}

void Level1::LoadNextLevel() const
{
    // Todo: Load-up next level.
    m_context->m_states->Add(std::make_unique<GameOver>(m_context));
}
