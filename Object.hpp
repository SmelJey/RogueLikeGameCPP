#pragma once
#include "Utility.hpp"

using namespace util;

class Object {
public:
    Object() : pos(), symbol(0) {};
    Object(Point position, char ch) : pos(position), symbol(ch) {};

    Point getPos() {
        return pos;
    }
    char getSym() {
        return symbol;
    }
protected:
    Point pos;
    char symbol;
    bool isOnScreen = true;
};