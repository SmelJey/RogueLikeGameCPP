#pragma once

#include "Enemy.hpp"
#include "Player.hpp"
#include "Utility.hpp"

class Skeleton : public Enemy {
public:
    Skeleton(Point position, const Player& player) : Enemy(position, 'S', player) {};

    void update(std::vector<std::string>& map) override {
        Enemy::update(map);
    }
private:



};