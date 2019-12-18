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

    Player(int maxHp, int dmg, int shotDmg, util::Point pos, int id = 0);

    void update(util::GameInfo& game) override;

    virtual void interact(Enemy&, util::GameInfo&) override;

    void swap(Player& src);

    Player& operator=(const Player& src);

private:
    void shoot(util::GameInfo& game);

    const std::vector<std::tuple<char, util::Point, char, util::Point>> moves = { { 'a', util::Point(-1, 0), '<', util::Point::Left() },
                                                                { 'w', util::Point(0, -1), '^', util::Point::Up() },
                                                                { 'd', util::Point(1, 0), '>', util::Point::Right() },
                                                                { 's', util::Point(0, 1), 'v', util::Point::Down() } };

    int lastDir = 0;
    int shotDamage = 2;
    int shots = 0;
    int shootingTimer = 0;
};