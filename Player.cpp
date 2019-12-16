#pragma once

#include "Entity.hpp"
#include "Utility.hpp"
#include "Projectile.hpp"
#include "Player.hpp"
#include "Object.hpp"

#include <curses.h>
#include <Windows.h>

#include <map>

Player::Player() : Entity('<', 10, 2, 0, util::Point(0, 0)), shots(0), shotDamage(2) {}

Player::Player(int maxHp, int dmg, int shotDmg, util::Point pos, int id) : Entity('<', maxHp, dmg, id, pos), shots(0), shotDamage(shotDmg) {}

std::pair<Object&, Object&> Player::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (this->hp <= 0)
        return collision;
    for (int i = 0; i < 4; i++) {
        if (GetKeyState(std::get<0>(moves[i])) & 0x8000) {
            auto newPos = this->pos + std::get<1>(moves[i]);
            lastDir = i;
            this->symbol = std::get<2>(moves[i]);
            if (!util::checkPoint(game.map, newPos))
                continue;
            if (game.map[newPos.y][newPos.x] != '#') {
                if (game.map[newPos.y][newPos.x] == '.') {
                    game[pos] = '.';
                    pos = newPos;
                } else {
                    game.map[pos.y][pos.x] = symbol;
                    return findCollision(newPos, game);
                }
            }
        }
    }
    if (GetKeyState('E') & 0x8000) {
        auto checkPoint = this->pos + std::get<3>(moves[lastDir]);
        if (util::checkPoint(game.map, checkPoint) && game[checkPoint] != '#') {
            if (game[checkPoint] != '.') {
                game.map[pos.y][pos.x] = symbol;
                return findCollision(checkPoint, game);
            }
            shoot(game);
        }
    }


    game.map[pos.y][pos.x] = symbol;
    return collision;
}

void Player::shoot(util::GameInfo& game) {
    if (util::checkPoint(game.map, this->pos + std::get<3>(moves[lastDir])))
        game.projectiles.push_back(std::unique_ptr<Entity>(new Projectile(game.getNextId(), this->pos + std::get<3>(moves[lastDir]),
            std::get<3>(moves[lastDir]), 'o', shotDamage, true)));
}

void Player::swap(Player& src) {
    std::swap(this->damage, src.damage);
    std::swap(this->enabled, src.enabled);
    std::swap(this->hp, src.hp);
    std::swap(this->lastDir, src.lastDir);
    std::swap(this->maxHp, src.maxHp);
    std::swap(this->pos, src.pos);
    std::swap(this->symbol, src.symbol);
}

Player& Player::operator=(const Player& src) {
    Player tmp(src);
    swap(tmp);
    return *this;
}