#pragma once

#include "Utility.hpp"
#include "Object.hpp"

using namespace util;

Object::Object() : pos(), symbol(0) {};
Object::Object(Point position, char ch) : pos(position), symbol(ch) {};

Point Object::getPos() const {
    return pos;
}
char Object::getSym() const {
    return symbol;
}
bool Object::isEnabled() const {
    return enabled;
}

void Object::draw(GameInfo& game) const {
    game[this->pos] = symbol;
}

bool operator<(const Object& lhs, const Object& rhs) {
    return lhs.pos < rhs.pos;
}
