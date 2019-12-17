#pragma once

#include "Utility.hpp"
#include "Portal.hpp"
#include "Item.hpp"

class MapGenerator {
public:
    MapGenerator(int seed = 0);

    void generateMap(std::vector<std::string> &map, util::GameInfo& game);

    int getSeed();

private:
    void fillRandom(util::GameInfo& game, double prob);

    void cellAutomatonSimulate(util::GameInfo& game);

    void generateMaze(std::vector<std::string> &map, double prob);

    int seed;
};