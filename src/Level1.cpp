#include "Level1.hpp"
#include "Level2.hpp"
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
    m_context->m_states->Add(std::make_unique<Level2>(m_context));
}
