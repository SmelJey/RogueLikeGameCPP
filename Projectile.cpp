#pragma once
#include "Entity.hpp"
#include "Object.hpp"
#include "Utility.hpp"
#include "Projectile.hpp"

class Enemy;

Projectile::Projectile(int id, util::Point pos, util::Point direction, char sym, int dmg, bool isPlayer) : Entity(sym, 1, dmg, id, pos), speed(direction), isPlayer(isPlayer) {}

void Projectile::update(util::GameInfo& game) {
    if (game[this->pos] != '.' && game[this->pos] != symbol) {
        enabled = false;
        if (game[this->pos] == '#') {
            return;
        }
        auto& tmp = findCollision(this->pos, game);
        tmp.interact(*this, game);
        return;
    }
    
    game[this->pos] = '.';
    auto newPos = pos + speed;
    if (!util::checkPoint(game.map, newPos) || game[newPos] == '#') {
        enabled = false;
        return;
    }

    if (game[newPos] != '.') {
        auto& tmp = findCollision(newPos, game);
        tmp.interact(*this, game);
        return;
    }
    this->pos = newPos;
    game[this->pos] = this->symbol;

    return;
}
