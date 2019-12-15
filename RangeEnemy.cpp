#pragma once

#include "RangeEnemy.hpp"
#include "Projectile.hpp"

RangeEnemy::RangeEnemy(int id, util::Point pos, const Entity& player, char sym) : Enemy(id, pos, sym, player) {}

std::pair<Object&, Object&> RangeEnemy::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (this->hp <= 0)
        return collision;
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
                    auto it = std::find_if(game.entities.begin(), game.entities.end(), [nextPos](std::unique_ptr<Entity>& el) { return el->getPos() == nextPos; });
                    game[pos] = symbol;
                    return std::pair<Object&, Object&>(dynamic_cast<Object&>(*this), *it->get());
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
    return collision;
}

void RangeEnemy::shoot(util::GameInfo& game) {
    if (util::checkPoint(game.map, this->pos + directions[lastDirection]))
        game.entities.push_back(std::unique_ptr<Entity>(new Projectile(game.getNextId(), this->pos + directions[lastDirection],\
            directions[lastDirection], '+', rangedDmg, false)));
}
