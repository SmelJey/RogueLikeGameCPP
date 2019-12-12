#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"

class Zombie : public Enemy {
public:
    Zombie(util::Point pos, const Entity& player);

};