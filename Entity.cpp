#pragma once
#include "Object.hpp"
#include "Utility.hpp"
#include "Entity.hpp"
#include "Projectile.hpp"

#include <vector>
#include <string>
#include <utility>


Entity::Entity(char sym, int maxHp, int dmg, int id, util::Point pos) : Object(sym, id, pos), maxHp(maxHp), damage(dmg), hp(maxHp) {}

int Entity::getHp() const {
    return hp;
}

void Entity::setHp(int hp) {
    this->hp = hp;
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

void Entity::interact(Projectile& obj, util::GameInfo& game) {
    this->getHit(obj.getDmg(), game);
    Object::interact(obj, game);
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

void Entity::getHit(int dmg, util::GameInfo& game) {
    hp -= dmg;
    if (hp <= 0 && enabled) {
        game[pos] = '.';
        enabled = false;
        pos = util::Point(-1, -1);
    }
       
}

Object& Entity::findCollision(util::Point pos, util::GameInfo& game) {
    auto it = std::find_if(game.entities.begin(), game.entities.end(), [pos](std::unique_ptr<Entity>& el) {
        return el->getPos() == pos;
        });
    if (it == game.entities.end()) {
        auto itp = std::find_if(game.projectiles.begin(), game.projectiles.end(), [pos](std::unique_ptr<Entity>& el) {
            return el->getPos() == pos;
        });
        if (itp == game.projectiles.end()) {
            throw;
        }
        return *itp->get();
    }
    return *it->get();
}
