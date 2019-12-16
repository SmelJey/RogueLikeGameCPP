#pragma once

#include "Utility.hpp"
#include "Object.hpp"

using namespace util;

Object::Object() : pos(), symbol(0), id(-1) {}
Object::Object(char ch, int id, Point position) : pos(position), symbol(ch), id(id) {}

Point Object::getPos() const {
    return pos;
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

bool operator<(const Object& lhs, const Object& rhs) {
    return lhs.pos < rhs.pos;
}

bool  Object::operator==(const Object& rhs) const {
    return this->id == rhs.id;
}

bool  Object::operator!=(const Object& rhs) const {
    return this->id != rhs.id;
}
