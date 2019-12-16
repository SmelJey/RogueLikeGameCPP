#pragma once

#include "Object.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Projectile.hpp"
#include "Utility.hpp"

#include <queue>
#include <utility>

namespace CSolvers {
    struct DoNothing {
        void operator()(Object*, Object*) {}
    };
    struct SelfDestruct {
        void operator()(Entity* active, Entity* passive, util::GameInfo& game) {
            active->getHit(active->getHp(), game);
            passive->getHit(active->getDmg(), game);
        }
    };
    struct Attack {
        void operator()(Entity* active, Entity* passive, util::GameInfo& game) {
            passive->getHit(active->getDmg(), game);
        }
    };
    struct Defend {
        void operator()(Entity* active, Entity* passive, util::GameInfo& game) {
            active->getHit(passive->getDmg(), game);
        }
    };
    struct Competitive {
        void operator()(Entity* active, Entity* passive, util::GameInfo& game) {
            active->getHit(passive->getDmg(), game);
            passive->getHit(active->getDmg(), game);
        }
    };
    // TODO: Strategies for items
}


class CollisionManager {
public:
    void addCollision(std::pair<Object&, Object&> col);

    void process(util::GameInfo& game);

    void tryToCollide(Object& active, Object& passive, util::GameInfo& game);

private:

    template<typename T1, typename T2>
    struct CollisionStrategy {
        CSolvers::DoNothing solver;
    };

    template<>
    struct CollisionStrategy<Enemy*, Player*> {
        CSolvers::Attack solver;
    };

    template<>
    struct CollisionStrategy<Player*, Enemy*> {
        CSolvers::Attack solver;
    };

    template<>
    struct CollisionStrategy<Projectile*, Entity*> {
        CSolvers::SelfDestruct solver;
    };

    std::queue<std::pair<Object&, Object&>> collisionQueue;
};