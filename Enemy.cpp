#pragma once

#include "Entity.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"

#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

std::pair<Object&, Object&> Enemy::update(util::GameInfo& game) {
    std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
    if (this->hp <= 0)
        return collision;
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
            auto it = std::find_if(game.entities.begin(), game.entities.begin(), [nextPos](std::unique_ptr<Entity>& el) { return el->getPos() == nextPos; });
            game[pos] = symbol;
            return std::pair<Object&, Object&>(dynamic_cast<Object&>(*this), *it->get());
        }
    }

    game[pos] = symbol;
    return collision;
}

Enemy::Enemy(util::Point pos, char sym, const Entity& player) : Entity(pos, sym), playerRef(player) {}


util::Point Enemy::bfs(const std::vector<std::string>& map, const util::Point& target, int distance) const {
    const util::Point d[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    std::queue<util::Point> q;
    std::queue<util::Point> newQ;

    q.push(this->pos);
    std::map<util::Point, util::Point> used;
    used[this->pos] = this->pos;

    util::Point res = this->pos;
    int dist = 0;

    while (!q.empty()) {
        if (dist > distance)
            break;

        swap(q, newQ);
        while (!newQ.empty()) {
            auto node = newQ.front();
            newQ.pop();
            if (node == target) {
                res = node;
                break;
            }

            for (int i = 0; i < 4; i++) {
                util::Point newNode = node + d[i];
                if (checkPoint(map, newNode) && !used.count(newNode) && (map[newNode.y][newNode.x] == '.' || map[newNode.y][newNode.x] == playerRef.getSym())) {
                    q.push(newNode);
                    used[newNode] = node;
                }
            }
        }
        dist++;
    }

    while (abs(res.x - this->pos.x) > 1 || abs(res.y - this->pos.y) > 1) {
        res = used[res];
    }

    return res;
}