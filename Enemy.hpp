#pragma once

#include "Entity.hpp"
#include "Utility.hpp"

#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

class Enemy : public Entity {
public:
    void update(std::vector<std::string>& map) override {
        if (abs(playerRef.getPos().x - pos.x) + abs(playerRef.getPos().y - pos.y) <= sightRange) {
            auto nextPos = bfs(map, playerRef.getPos(), sightRange);
            if (nextPos == this->pos)
                nextPos.randomize(map);
            this->pos = nextPos;
        } else {
            this->pos.randomize(map);
        }

        map[pos.y][pos.x] = symbol;
    }

protected:
    Enemy(Point pos, char sym, const Player& player) : Entity(pos, sym), playerRef(player) {}

    const Player& playerRef;

    int sightRange = 15;

    Point bfs(const std::vector<std::string>& map, const Point& target, int distance) const {
        const Point d[] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

        std::queue<Point> q;
        std::queue<Point> newQ;

        q.push(this->pos);
        std::map<Point, Point> used;
        used[this->pos] = this->pos;

        Point res = this->pos;
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

                for (int i = 0; i < 8; i++) {
                    Point newNode = node + d[i];
                    if (checkPoint(map, newNode) && !used.count(newNode) && map[newNode.y][newNode.x] == '.') {
                        q.push(newNode);
                        used[newNode] = node;
                    }
                }
            }
            dist++;
        }
        
        while (used[res] != this->pos) {
            res = used[res];
        }

        return res;
    }
};