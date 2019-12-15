#pragma once

#include "Enemy.hpp"

class RangeEnemy : public Enemy {
public:
    RangeEnemy(int id, util::Point pos, const Entity& player, char sym);

    std::pair<Object&, Object&> update(util::GameInfo& game) override;

protected:

    void shoot(util::GameInfo& game);

    int lastDirection = 0;
    int shootingCd = 8;
    int shootingTimer = shootingCd;

    int rangedDmg = 2;

    util::Point directions[4] = { util::Point::Left(), util::Point::Up(), util::Point::Right(), util::Point::Down() };
};