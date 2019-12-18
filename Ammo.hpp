#pragma once

#include "Item.hpp"

class Ammo : public Item {
public:
    Ammo(char sym, int ammoRestoration, int id = -1, util::Point pos = util::Point(-1, -1));

    void virtual use(Player& player, util::GameInfo& game) override;

protected:
    int ammo;
};