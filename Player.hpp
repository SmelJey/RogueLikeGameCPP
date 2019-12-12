#pragma once

#include "Entity.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"

#include <curses.h>
#include <Windows.h>

#include <map>


class Player : public Entity {
public:
    Player();

    explicit Player(util::Point pos);

    std::pair<Object&, Object&> update(util::GameInfo& game) override;

private:
    void shoot(util::GameInfo& game);

    const std::vector<std::tuple<char, util::Point, char, util::Point>> moves = { { 'A', util::Point(-1, 0), '<', util::Point::Left() },
                                                                { 'W', util::Point(0, -1), '^', util::Point::Up() },
                                                                { 'D', util::Point(1, 0), '>', util::Point::Right() },
                                                                { 'S', util::Point(0, 1), 'v', util::Point::Down() } };

    int lastDir = 0;
};