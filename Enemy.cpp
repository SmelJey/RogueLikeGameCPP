#pragma once

#include "Entity.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"

#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

Enemy::Enemy(char sym, int maxHp, int moveCd, int dmg, int sightRange, const Entity& player, int id, util::Point pos)
    : Entity(sym, maxHp, dmg, id, pos), moveCd(moveCd), sightRange(sightRange), playerRef(player) {}

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
                if (checkPoint(map, newNode) && !used.count(newNode) && (map[newNode.y][newNode.x] == '.' ||
                    map[newNode.y][newNode.x] == 'o' || map[newNode.y][newNode.x] == playerRef.getSym())) {
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