#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"
#include "Player.hpp"

class Zombie : public Enemy {
public:
    Zombie(Point pos, const Player& player) : Enemy(pos, 'Z', player) {}

};