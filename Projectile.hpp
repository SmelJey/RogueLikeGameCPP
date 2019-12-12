#pragma once
#include "Entity.hpp"
#include "Object.hpp"
#include "Utility.hpp"
#include "Enemy.hpp"

class Enemy;

class Projectile : public Entity {
public:
    Projectile(Point pos, Point direction, char sym, bool isPlayer) : Entity(pos, sym), speed(direction), isPlayer(isPlayer) {
        maxHp = 1;
    }

    std::pair<Object&, Object&> update(GameInfo& game) {
        std::pair<Object&, Object&> collision(dynamic_cast<Object&>(*this), dynamic_cast<Object&>(*this));
        game[this->pos] = '.';
        auto newPos = pos + speed;
        if (!checkPoint(game.map, newPos)){
            enabled = false;
            return collision;
        }
        if (game[newPos] == '#') {
            enabled = false;
            return collision;
        }

        if (game[newPos] != '.') {
            auto it = std::find_if(game.entities.begin(), game.entities.begin(), [newPos](std::unique_ptr<Entity>& el) { return el->getPos() == newPos; });
            return std::pair<Object&, Object&>(dynamic_cast<Object&>(*this), *it->get());
        }
        this->pos = newPos;
        game[this->pos] = this->symbol;

        return collision;
    }

private:
    Point speed;
    bool isPlayer;
};