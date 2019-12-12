#include "CollisionManager.hpp"

#include <queue>
#include <utility>


void CollisionManager::addCollision(std::pair<Object&, Object&> col) {
    collisionQueue.push(col);
}

void CollisionManager::process(util::GameInfo& game) {
    while (!collisionQueue.empty()) {
        tryToCollide(collisionQueue.front().first, collisionQueue.front().second);
        collisionQueue.pop();
    }
}

void CollisionManager::tryToCollide(Object& active, Object& passive) {

}
