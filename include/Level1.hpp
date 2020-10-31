#pragma once 

#include "Level.hpp"

class Level1 : public Level
{
public:
    Level1(const std::shared_ptr<Context>& context);
    ~Level1();

    void LoadNextLevel() const override;
};
