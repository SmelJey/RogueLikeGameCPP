#include "HealPotion.hpp"
#include "Player.hpp"

HealPotion::HealPotion(char sym, int hpRestoration, int id, util::Point pos) : Item(sym, id, pos), hpRestoration(hpRestoration) {}

void HealPotion::use(Player& player, util::GameInfo& game) {
    player.getHit(-hpRestoration, game);
    enabled = false;
    game[pos] = '.';
    pos = util::Point(-1, -1);
}
