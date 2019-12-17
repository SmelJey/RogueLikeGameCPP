#pragma once

#include "RangeEnemy.hpp"
#include "Projectile.hpp"

RangeEnemy::RangeEnemy(char sym, int maxHp, int moveCd, int dmg, int sightRange, const Entity& player, int shotCd, int shotDmg, int id, util::Point pos)
    : Enemy(sym, maxHp, moveCd, dmg, sightRange, player, id, pos), shotDamage(shotDmg), shootingCd(shotCd) {}

RangeEnemy::RangeEnemy(int id, util::Point pos, const RangeEnemy& src)
    : RangeEnemy(src.symbol, src.maxHp, src.moveCd, src.damage, src.sightRange, src.playerRef, src.shootingCd, src.shotDamage, id, pos) {
    enabled = true;
}

void RangeEnemy::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (this->hp <= 0)
        return;
    moveTimer--;
    if (moveTimer <= 0) {
        moveTimer = moveCd;
        util::Point nextPos(this->pos);
        lastDirection = rand() % 4;
        nextPos += directions[lastDirection];
        if (util::checkPoint(game.map, nextPos)) {
            if (game[nextPos] != '#') {
                if (game[nextPos] == '.') {
                    game[pos] = '.';
                    this->pos = nextPos;
                } else {
                    game[pos] = symbol;
                    auto& tmp = findCollision(nextPos, game);
                    tmp.interact(*this, game);
                    return;
                }
            }
        }
    }
    
    shootingTimer--;
    if (shootingTimer == 0) {
        shootingTimer = shootingCd;
        shoot(game);
    }
    game[pos] = symbol;
    return;
}

void RangeEnemy::shoot(util::GameInfo& game) {
    if (util::checkPoint(game.map, this->pos + directions[lastDirection]))
        game.projectiles.push_back(std::unique_ptr<Entity>(new Projectile(game.getNextId(), this->pos + directions[lastDirection],\
            directions[lastDirection], '*', shotDamage, false)));
}
