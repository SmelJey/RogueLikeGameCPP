#include "Portal.hpp"

Portal::Portal(char sym, int id, util::Point pos) : Item(sym, id, pos) {}

void Portal::use(Player& playerRef, util::GameInfo& game) {
    game.isWin = true;
    enabled = false;
    game[pos] = '.';
    pos = util::Point(-1, -1);
}
