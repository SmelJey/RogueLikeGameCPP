#pragma once

#include "Item.hpp"

class Portal : public Item {
public:
    Portal(char sym, int id = -1, util::Point pos = util::Point(-1, -1));

    void use(Player& playerRef, util::GameInfo& game) override;

};