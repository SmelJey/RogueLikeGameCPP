#pragma once
#include "Object.hpp"
#include "Utility.hpp"
#include "Entity.hpp"

#include <vector>
#include <string>
#include <utility>


Entity::Entity(util::Point coords, char symb) : Object(coords, symb), hp(maxHp) {};

int Entity::getHp() const {
    return hp;
}

int Entity::getMaxHp() const {
    return maxHp;
}
void Entity::setMaxHp(int hp) {
    this->maxHp = hp;
}

int Entity::getDmg() const {
    return damage;
}
void Entity::setDmg(int dmg) {
    this->damage = dmg;
}

util::Point Entity::getPos() const {
    return pos;
}
void Entity::setPos(util::Point pos) {
    this->pos = pos;
}

void Entity::getHit(int dmg) {
    hp -= dmg;
}
