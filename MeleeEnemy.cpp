#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"

#include "MeleeEnemy.hpp"

MeleeEnemy::MeleeEnemy(int id, util::Point pos, const Entity& player, char sym) : Enemy(id, pos, sym, player) {}
std::pair<Object&, Object&> MeleeEnemy::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (this->hp <= 0)
        return collision;
    moveTimer--;
    if (moveTimer <= 0) {
        moveTimer = moveCd;
        util::Point nextPos(this->pos);

        if (abs(playerRef.getPos().x - pos.x) + abs(playerRef.getPos().y - pos.y) <= sightRange) {
            nextPos = bfs(game.map, playerRef.getPos(), sightRange);
            if (nextPos == this->pos)
                nextPos.randomize(game.map);
        } else {
            nextPos.randomize(game.map);
        }

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

    game[pos] = symbol;
    return collision;
}
