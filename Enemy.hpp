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
    std::pair<Object&, Object&> update(util::GameInfo& game) override;

protected:
    Enemy(util::Point pos, char sym, const Entity& player);

    const Entity& playerRef;

    int sightRange = 15;

    util::Point bfs(const std::vector<std::string>& map, const util::Point& target, int distance) const;
};