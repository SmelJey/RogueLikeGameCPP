#pragma once

#include "Entity.hpp"
#include "Utility.hpp"

#include <curses.h>
#include <Windows.h>

#include <map>


class Player : public Entity {
public:
    Player() : Entity(Point(0, 0), 'P') {}

    explicit Player(Point pos) : Entity(pos, 'P') { }

    void update(std::vector<std::string>& map) override {
        for (auto it = moves.begin(); it != moves.end(); it++) {
            if (GetKeyState(it->first) & 0x8000) {
                auto newPos = pos + it->second;
                if (!checkPoint(map, newPos))
                    continue;
                if (map[newPos.y][newPos.x] != '#') {
                    if (map[newPos.y][newPos.x] == '.') {
                        pos = newPos;
                    }
                }
            }
        }

        map[pos.y][pos.x] = symbol;
    }



private:
    /*void collide(Enemy& col) {
        attack(col);
    }*/

    const std::map<char, Point> moves = { { 'A', Point(-1, 0) },
                                    { 'W', Point(0, -1) },
                                    { 'D', Point(1, 0) },
                                    { 'S', Point(0, 1) } };

};