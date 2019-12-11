#pragma once
#include "Object.hpp"

#include <vector>
#include <string>

class Entity : public Object {
public:
    Entity(Point coords, char symb) : Object(coords, symb), hp(maxHp) {};

    virtual void update(std::vector<std::string>& map) = 0;

    int getHp() const {
        return hp;
    }

    int getMaxHp() const {
        return maxHp;
    }
    void setMaxHp(int hp) {
        this->maxHp = hp;
    }

    int getDmg() const {
        return damage;
    }
    void setDmg(int dmg) {
        this->damage = dmg;
    }

    Point getPos() const {
        return pos;
    }
    void setPos(Point pos) {
        this->pos = pos;
    }

    void getHit(int dmg) {
        hp -= dmg;
    }

    bool operator<(const Entity& rhs) {
        return this->pos < rhs.pos;
    }

protected:

    void attack(Entity& target) {
        target.getHit(damage);
    }

    int maxHp = 10;
    int hp;
    
    int damage = 1;
};