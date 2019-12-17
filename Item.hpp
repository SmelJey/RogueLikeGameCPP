#pragma once

#include "Object.hpp"
#include "Utility.hpp"
#include "Entity.hpp"

class Item : public Object {
public:
    Item(char sym, int id = -1, util::Point pos = util::Point(-1, -1));

    virtual void interact(Player&, util::GameInfo&) override;

    virtual void use(Player& playerRef, util::GameInfo& game) = 0;
};