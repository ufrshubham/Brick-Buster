#pragma once

#include "Level1.hpp"

class Level2 : public Level
{
public:
    Level2(const std::shared_ptr<Context>& context);
    ~Level2();

    void LoadNextLevel() const override;
};

