#pragma once

#include "Utility.hpp"
#include "Object.hpp"
#include "Projectile.hpp"

using namespace util;

Object::Object() : pos(), symbol(0), id(-1) {}
Object::Object(char ch, int id, Point position) : pos(position), symbol(ch), id(id) {}

util::Point Object::getPos() const {
    return pos;
}
void Object::setPos(util::Point pos) {
    this->pos = pos;
}

char Object::getSym() const {
    return symbol;
}

bool Object::isEnabled() const {
    return enabled;
}

void Object::setEnabled(bool val) {
    enabled = val;
}

void Object::draw(GameInfo& game) const {
    game[this->pos] = symbol;
}

void Object::interact(Enemy&, util::GameInfo&) {}
void Object::interact(Player&, util::GameInfo&) {}
void Object::interact(Projectile& obj, util::GameInfo& game) {
    obj.getHit(obj.getHp(), game);
}

bool operator<(const Object& lhs, const Object& rhs) {
    return lhs.pos < rhs.pos;
}

bool Object::operator==(const Object& rhs) const {
    return this->id == rhs.id;
}

bool Object::operator!=(const Object& rhs) const {
    return this->id != rhs.id;
}
