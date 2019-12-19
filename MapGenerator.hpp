#pragma once

#include <map>

#include "Utility.hpp"
#include "Portal.hpp"
#include "Item.hpp"

class MapGenerator {
public:
    void generateMap(std::vector<std::string> &map, util::GameInfo& game, int lvl) const;

private:
    void initAdjList(std::vector<std::string>& map, std::map<util::Point, std::vector<util::Point>>& adjList) const;

    void makeConnected(std::vector<std::string>& map, std::map<util::Point, std::vector<util::Point>>& adjList) const;

    void fillRandom(std::vector<std::string>& map, double prob) const;

    void cellAutomatonSimulate(std::vector<std::string>& map) const;

    void generateMaze(std::vector<std::string> &map, double prob) const;

    void generateCave(std::vector<std::string>& map, double prob) const;

    util::Point dfs(util::Point node, std::map<util::Point, std::vector<util::Point>>& adjList, std::set<util::Point>& used, std::vector<std::string>& map) const;
};