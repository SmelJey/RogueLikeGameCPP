#include "MapGenerator.hpp"
#include <algorithm>
#include <vector>
#include <set>
#include <map>

#include "Portal.hpp"
#include "HealPotion.hpp"
#include "Ammo.hpp"
#include "Item.hpp"


void MapGenerator::generateMap(std::vector<std::string> &map, util::GameInfo& game, int lvl) const {
    size_t portalX = 1, portalY = 1;

    while (portalX == 1 && portalY == 1) {
        game.items.clear();
        if ((lvl - 1) % 10 < 5)
            generateMaze(map, 0.75);
        else
            generateCave(map, 0.45);

        for (int i = map.size() - 1; i >= 0; i--) {
            for (int j = map[i].size() - 1; j >= 0; j--) {
                if (map[i][j] == '.') {
                    portalY = i;
                    portalX = j;
                    game.items.push_back(std::unique_ptr<Object>(new Portal('Q', game.getNextId(), util::Point(j, i))));
                    i = 0;
                    break;
                }
            }
        }
    }
   

    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '.' && !(i == portalY && j == portalX) && !(i == 1 && j == 1)) {
                if (rand() % 100 / 100.0 < game.itemsProps["AmmoSpawnRate"]) {
                    game.items.push_back(std::unique_ptr<Object>(
                        new Ammo('%', static_cast<int>(game.itemsProps["AmmoRestoration"]), game.getNextId(), util::Point(j, i))));
                } else if (rand() % 100 / 100.0 < game.itemsProps["HealPotionSpawnRate"]) {
                    game.items.push_back(std::unique_ptr<Object>(
                        new HealPotion('+', static_cast<int>(game.itemsProps["HealPotionRestoration"]), game.getNextId(), util::Point(j, i))));
                }
            }
        }
    }
}

void MapGenerator::generateMaze(std::vector<std::string> &map, double prob) const {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            map[i][j] = '#';
        }
    }

    const util::Point d[2] = { util::Point(-2, 0), util::Point(0, -2) };

    std::map<util::Point, std::vector<util::Point>> adjList;

    map[1][1] = '.';

    for (size_t i = 1; i < map.size(); i += 2) {
        for (size_t j = 1; j < map[i].size(); j += 2) {
            auto node = util::Point(j, i);
            double p = (rand() % 100) / 100.0;
            if (p <= prob) {
                map[node.y][node.x] = '.';
                for (size_t k = 0; k < 2; k++) {
                    auto check = node + d[k];
                    if (util::checkPoint(map, check) && map[check.y][check.x] == '.') {
                        adjList[node].push_back(check);
                        adjList[check].push_back(node);
                    }
                }
            }
        }
    }

    std::set<std::pair<int, util::Point>> q;
    std::set<util::Point> used;

    util::Point lastPoint(1, 1);

    for (size_t i = 1; i < map.size(); i += 2) {
        for (size_t j = 1; j < map[i].size(); j += 2) {
            auto node = util::Point(j, i);
            if (map[node.y][node.x] == '.' && used.count(node) == 0) {
                for (int k = std::min(lastPoint.y, (int)i); k < std::max(lastPoint.y, (int)i); k += 2) {
                    map[k][j] = '.';
                    map[k + 1ll][j] = '.';
                    used.insert(util::Point(j, k));
                }
                map[std::max(lastPoint.y, (int)i)][j] = '.';
                used.insert(util::Point(j, std::max(lastPoint.y, (int)i)));
                for (int k = std::min(lastPoint.x, (int)j); k <= std::max(lastPoint.x, (int)j); k += 2) {
                    map[i][k] = '.';
                    map[i][k + 1ll] = '.';
                    used.insert(util::Point(k, i));
                }
                used.erase(node);
                lastPoint = dfs(node, adjList, used, map);
            }
        }
    }

    for (size_t i = 3; i < map.size(); i++) {
        for (size_t j = 3; j < map[i].size(); j++) {
            if (map[i][j]           == '.'
                && map[i - 2][j]    == '.'
                && map[i][j - 2]    == '.'
                && map[i - 2][j - 2] == '.') {
                double p = (rand() % 100) / 100.0;
                if (p > prob)
                    continue;
                for (int i0 = -2; i0 < 1; i0++) {
                    for (int j0 = -2; j0 < 1; j0++) {
                        map[i + i0][j + j0] = '.';
                    }
                }
            }
        }
    }
}

void MapGenerator::generateCave(std::vector<std::string>& map, double prob) const {
    fillRandom(map, prob);

    for (int i = 0; i < 4; i++) {
        cellAutomatonSimulate(map);
    }

    std::map<util::Point, std::vector<util::Point>> adjList;
    initAdjList(map, adjList);

    makeConnected(map, adjList);
}

void MapGenerator::initAdjList(std::vector<std::string>& map, std::map<util::Point, std::vector<util::Point>>& adjList) const {
    const int dx[4] = { 0, 0, 1, -1 };
    const int dy[4] = { 1, -1, 0, 0 };

    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            for (size_t k = 0; k < 4; k++) {
                if (i + dy[k] >= 0 && i + dy[k] < map.size()) {
                    if (j + dx[k] >= 0 && j + dx[k] < map[i].size()) {
                        if (map[i + dy[k]][j + dx[k]] == '.') {
                            adjList[util::Point(j, i)].push_back(util::Point(j + dx[k], i + dy[k]));
                        }
                    }
                }
            }
        }
    }
}

void MapGenerator::makeConnected(std::vector<std::string>& map, std::map<util::Point, std::vector<util::Point>>& adjList) const {
    std::set<std::pair<int, util::Point>> q;
    std::set<util::Point> used;

    util::Point lastPoint(0, 0);

    for (size_t i = 1; i < map.size(); i++) {
        for (size_t j = 1; j < map[i].size(); j++) {
            auto node = util::Point(j, i);
            if (map[node.y][node.x] == '.' && used.count(node) == 0) {
                for (int k = std::min(lastPoint.y, (int)i); k < std::max(lastPoint.y, (int)i); k++) {
                    map[k][j] = '.';
                    used.insert(util::Point(j, k));
                }
                map[std::max(lastPoint.y, (int)i)][j] = '.';
                used.insert(util::Point(j, std::max(lastPoint.y, (int)i)));
                for (int k = std::min(lastPoint.x, (int)j); k <= std::max(lastPoint.x, (int)j); k++) {
                    map[i][k] = '.';
                    used.insert(util::Point(k, i));
                }
                used.erase(node);
                lastPoint = dfs(node, adjList, used, map);
            }
        }
    }
}

void MapGenerator::fillRandom(std::vector<std::string>& map, double prob) const {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            double p = (rand() % 100) / 100.0;
            if (p < prob) {
                map[i][j] = '#';
            } else {
                map[i][j] = '.';
            }
        }
    }
}

void MapGenerator::cellAutomatonSimulate(std::vector<std::string>& map) const {
    auto copyMap(map);
    for (int i = 0; i < static_cast<int>(map.size()); i++) {
        for (int j = 0; j < static_cast<int>(map[i].size()); j++) {
            int count = 0;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) {
                        continue;
                    }
                    if (i + di >= 0 && i + di < static_cast<int>(map.size())) {
                        if (j + dj >= 0 && j + dj < static_cast<int>(map[i + di].size())) {
                            if (map[di + i][dj + j] == '#')
                                count++;
                        }
                    }
                }
            }
            if (map[i][j] == '.' && count > 4)
                copyMap[i][j] = '#';
            else if (map[i][j] == '#' && count < 3) {
                copyMap[i][j] = '.';
            }
        }
    }
    map.swap(copyMap);
}

util::Point MapGenerator::dfs(util::Point node, std::map<util::Point, std::vector<util::Point>>& adjList, std::set<util::Point>& used, std::vector<std::string>& map) const {
    used.insert(node);
    util::Point farestPoint(node);
    for (auto& v : adjList[node]) {
        if (used.count(v) == 0) {
            map[(node.y + v.y) / 2][(node.x + v.x) / 2] = '.';
            auto res = dfs(v, adjList, used, map);
            if (std::abs(node.x - res.x) + std::abs(node.y - res.y) > std::abs(node.x - farestPoint.x) + std::abs(node.y - farestPoint.y)) {
                farestPoint = res;
            }
        }
    }
    return farestPoint;
}