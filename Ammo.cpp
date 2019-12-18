#include "Ammo.hpp"
#include "Player.hpp"

Ammo::Ammo(char sym, int ammoRestoration, int id, util::Point pos) : Item(sym, id, pos), ammo(ammoRestoration) {}

void Ammo::use(Player& player, util::GameInfo& game) {
    player.setShots(player.getShots() + this->ammo);
    enabled = false;
    game[pos] = '.';
    pos = util::Point(-1, -1);
}
