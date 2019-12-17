#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"

class MeleeEnemy : public Enemy {
public:
    MeleeEnemy(char sym, int maxHp, int moveCd, int dmg, int sightRange, const Entity& player, int id = -1, util::Point pos = util::Point(-1, -1));
    MeleeEnemy(int id, util::Point pos, const MeleeEnemy& src);

    void update(util::GameInfo& game) override;
};