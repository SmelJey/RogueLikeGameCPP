#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"

class MeleeEnemy : public Enemy {
public:
    MeleeEnemy(int id, util::Point pos, const Entity& player, char sym);

    std::pair<Object&, Object&> update(util::GameInfo& game) override;
};