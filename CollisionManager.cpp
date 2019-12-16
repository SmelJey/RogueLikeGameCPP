#include "CollisionManager.hpp"
#include "Object.hpp"
#include "Entity.hpp"
#include "Enemy.hpp"

#include <queue>
#include <utility>

void CollisionManager::addCollision(std::pair<Object&, Object&> col) {
    collisionQueue.push(col);
}

void CollisionManager::process(util::GameInfo& game) {
    while (!collisionQueue.empty()) {
        if (collisionQueue.front().first == collisionQueue.front().second) {
            collisionQueue.pop();
            continue;
        }
            
        tryToCollide(collisionQueue.front().first, collisionQueue.front().second, game);
        collisionQueue.pop();
    }
}

void CollisionManager::tryToCollide(Object& active, Object& passive, util::GameInfo& game) {
    if (dynamic_cast<Projectile*>(&active) != nullptr) {
        if (dynamic_cast<Entity*>(&passive) != nullptr) {
            CollisionStrategy<Projectile*, Entity*>().solver(dynamic_cast<Entity*>(&active),
                dynamic_cast<Entity*>(&passive), game);
            return;
        }
    } else if (dynamic_cast<Player*>(&active) != nullptr) {
        if (dynamic_cast<Enemy*>(&passive) != nullptr) {
            CollisionStrategy<Player*, Enemy*>().solver(dynamic_cast<Entity*>(&active),
                dynamic_cast<Entity*>(&passive), game);
            return;
        }
    } else if (dynamic_cast<Enemy*>(&active) != nullptr) {
        if (dynamic_cast<Player*>(&passive) != nullptr) {
            CollisionStrategy<Enemy*, Player*>().solver(dynamic_cast<Entity*>(&active),
                dynamic_cast<Entity*>(&passive), game);
            return;
        }
    }
    CollisionStrategy<Object*, Object*>().solver(&active, &passive);
    return;
}
