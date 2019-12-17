#pragma once

#include "Enemy.hpp"
#include "Utility.hpp"

#include "MeleeEnemy.hpp"

MeleeEnemy::MeleeEnemy(char sym, int maxHp, int moveCd, int dmg, int sightRange, const Entity& player, int id, util::Point pos)
    : Enemy(sym, maxHp, moveCd, dmg, sightRange, player, id, pos) {}

MeleeEnemy::MeleeEnemy(int id, util::Point pos, const MeleeEnemy& src)
    : MeleeEnemy(src.symbol, src.maxHp, src.moveCd, src.damage, src.sightRange, src.playerRef, id, pos) {
    enabled = true;
}

void MeleeEnemy::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (this->hp <= 0)
        return;
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
                game[pos] = symbol;
                auto& tmp = findCollision(nextPos, game);
                tmp.interact(*this, game);
                return;
            }
        }
    }

    game[pos] = symbol;
    return;
}
