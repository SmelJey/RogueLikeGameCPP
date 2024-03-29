#pragma once

#include "Object.hpp"
#include "Entity.hpp"
#include "Utility.hpp"

#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class Enemy : public Entity {
public:
    Enemy(char sym, int maxHp, int moveCd, int dmg, int sightRange, const Entity& player, int id = -1, util::Point pos = util::Point(-1, -1));

    virtual void interact(Player&, util::GameInfo&) override;

protected:
    util::Point bfs(const std::vector<std::string>& map, const util::Point& target, int distance) const;

    void tryToMove(util::Point newPos, util::GameInfo& game);

    const Entity& playerRef;

    int sightRange = 15;
    int moveCd = 2;
    int moveTimer = 0;
};