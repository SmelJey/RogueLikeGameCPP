#pragma once
#include "Entity.hpp"
#include "Object.hpp"
#include "Utility.hpp"
#include "Projectile.hpp"

class Enemy;

Projectile::Projectile(int id, util::Point pos, util::Point direction, char sym, int dmg, bool isPlayer) : Entity(sym, 1, dmg, id, pos), speed(direction), isPlayer(isPlayer) {}

std::pair<Object&, Object&> Projectile::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (game[this->pos] != '.' && game[this->pos] != symbol) {
        enabled = false;
        if (game[this->pos] == '#') {
            return collision;
        }
        return findCollision(this->pos, game);
    }
    
    game[this->pos] = '.';
    auto newPos = pos + speed;
    if (!util::checkPoint(game.map, newPos)) {
        enabled = false;
        return collision;
    }
    if (game[newPos] == '#') {
        enabled = false;
        return collision;
    }

    if (game[newPos] != '.') {
        return findCollision(newPos, game);
    }
    this->pos = newPos;
    game[this->pos] = this->symbol;

    return collision;
}
