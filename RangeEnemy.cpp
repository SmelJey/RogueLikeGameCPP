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
    if (this->hp <= 0)
        return;
    moveTimer--;
    if (moveTimer <= 0) {
        moveTimer = moveCd;
        lastDirection = rand() % 4;
        util::Point nextPos(this->pos + directions[lastDirection]);
        tryToMove(nextPos, game);
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
