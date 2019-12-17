#pragma once

#include "Enemy.hpp"

class RangeEnemy : public Enemy {
public:
    RangeEnemy(char sym, int maxHp, int moveCd, int dmg, int sightRange, const Entity& player, int shootingCd, int shotDmg, int id = -1, util::Point pos = util::Point(-1, -1));

    RangeEnemy(int id, util::Point pos, const RangeEnemy& src);

    void update(util::GameInfo& game) override;

protected:

    void shoot(util::GameInfo& game);

    int lastDirection = 0;
    int shootingCd = 8;
    int shootingTimer = shootingCd;

    int shotDamage = 2;

    util::Point directions[4] = { util::Point::Left(), util::Point::Up(), util::Point::Right(), util::Point::Down() };
};