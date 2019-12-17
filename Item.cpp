#include "Item.hpp"

Item::Item(char sym, int id, util::Point pos) : Object(sym, id, pos) {}

void Item::interact(Player& player, util::GameInfo& game) {
    use(player, game);
}
