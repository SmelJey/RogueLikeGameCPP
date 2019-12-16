#pragma once
#include "Entity.hpp"
#include "Object.hpp"
#include "Utility.hpp"

class Projectile : public Entity {
public:
    Projectile(int id, util::Point pos, util::Point direction, char sym, int dmg, bool isPlayer);

    std::pair<Object&, Object&> update(util::GameInfo& game);

private:
    util::Point speed;
    bool isPlayer;
};