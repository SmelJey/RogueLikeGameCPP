#pragma once

#include "Item.hpp"
#include "Utility.hpp"

class HealPotion : public Item {
public:
    HealPotion(char sym, int hpRestoration, int id = -1, util::Point pos = util::Point(-1, -1));

    void virtual use(Player& player, util::GameInfo& game) override;

protected:
    int hpRestoration;
};