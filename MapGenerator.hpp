#pragma once

#include "Utility.hpp"
#include "Portal.hpp"
#include "Item.hpp"

class MapGenerator {
public:
    void generateMap(std::vector<std::string> &map, util::GameInfo& game) const;

private:
    void fillRandom(util::GameInfo& game, double prob) const;

    void cellAutomatonSimulate(util::GameInfo& game) const;

    void generateMaze(std::vector<std::string> &map, double prob) const;

    util::Point dfs(util::Point node, std::map<util::Point, std::vector<util::Point>>& adjList, std::set<util::Point>& used, std::vector<std::string>& map) const;
};