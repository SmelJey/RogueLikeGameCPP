#pragma once

#include "Utility.hpp"

class Object {
public:
    Object();
    Object(util::Point position, char ch);

    util::Point getPos() const;
    char getSym() const;
    bool isEnabled() const;

    void draw(util::GameInfo& game) const;

    friend bool operator<(const Object& lhs, const Object& rhs);
protected:
    util::Point pos;
    char symbol;
    bool enabled = true;
};