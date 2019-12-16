#pragma once

#include "Utility.hpp"

class MapGenerator {
public:
    MapGenerator(int seed = 0);

    void generateMap(std::vector<std::string> &map);

    int getSeed();

private:
    void fillRandom(util::GameInfo& game, double prob);

    void cellAutomatonSimulate(util::GameInfo& game);

    void generateMaze(std::vector<std::string> &map, double prob);

    int seed;
};