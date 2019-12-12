#pragma once

#include "Object.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "Utility.hpp"

#include <queue>
#include <utility>

class CollisionManager {
public:
    void addCollision(std::pair<Object&, Object&> col) {
        collisionQueue.push(col);
    }

    void process(GameInfo& game) {
        while (!collisionQueue.empty()) {
            tryToCollide(collisionQueue.front().first, collisionQueue.front().second);
            collisionQueue.pop();
        }
    }

    void tryToCollide(Object& active, Object& passive) {

    }

private:
    std::queue<std::pair<Object&, Object&>> collisionQueue;
};