#pragma once

#include "Entity.hpp"
#include "Utility.hpp"
#include "Projectile.hpp"
#include "Player.hpp"
#include "Object.hpp"

#include <curses.h>
#include <Windows.h>

#include <map>

Player::Player() : Entity(util::Point(0, 0), '<') {}

Player::Player(util::Point pos) : Entity(pos, '<') { }

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
                    auto it = std::find_if(game.entities.begin(), game.entities.begin(), [newPos](std::unique_ptr<Entity>& el) { return el->getPos() == newPos; });
                    game.map[pos.y][pos.x] = symbol;
                    return std::pair<Object&, Object&>(dynamic_cast<Object&>(*this), *it->get());
                }
            }
        }
    }
    if (GetKeyState('E') & 0x8000) {
        auto checkPoint = this->pos + std::get<3>(moves[lastDir]);
        if (game[checkPoint] != '.') {
            auto it = std::find_if(game.entities.begin(), game.entities.begin(), [checkPoint](std::unique_ptr<Entity>& el) { return el->getPos() == checkPoint; });
            game.map[pos.y][pos.x] = symbol;
            return std::pair<Object&, Object&>(dynamic_cast<Object&>(*this), *it->get());
        }
        shoot(game);
    }


    game.map[pos.y][pos.x] = symbol;
    return collision;
}

void Player::shoot(util::GameInfo& game) {
    game.entities.push_back(std::unique_ptr<Entity>(new Projectile(this->pos, std::get<3>(moves[lastDir]), 'o', true)));
}
