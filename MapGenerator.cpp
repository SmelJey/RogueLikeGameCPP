#include "MapGenerator.hpp"
#include <algorithm>
#include <vector>
#include <set>
#include <map>

MapGenerator::MapGenerator(int seed) : seed(seed) {
    srand(seed);
}

void MapGenerator::generateMap(std::vector<std::string> &map) {
    generateMaze(map, 0.75);
}

void MapGenerator::randomSpawn(util::GameInfo& game) {

}

int MapGenerator::getSeed() {
    return seed;
}

util::Point dfs(util::Point node, std::map<util::Point, std::vector<util::Point>>& adjList, std::set<util::Point>& used, std::vector<std::string> &map) {
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

void MapGenerator::generateMaze(std::vector<std::string> &map, double prob) {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            map[i][j] = '#';
        }
    }

    const util::Point d[2] = { util::Point(-2, 0), util::Point(0, -2) };

    std::map<util::Point, std::vector<util::Point>> adjList;

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
                    map[k + 1][j] = '.';
                    used.insert(util::Point(j, k));
                }
                map[std::max(lastPoint.y, (int)i)][j] = '.';
                used.insert(util::Point(j, std::max(lastPoint.y, (int)i)));
                for (int k = std::min(lastPoint.x, (int)j); k < std::max(lastPoint.x, (int)j); k += 2) {
                    map[i][k] = '.';
                    map[i][k + 1] = '.';
                    used.insert(util::Point(k, i));
                }
                used.erase(node);
                lastPoint = dfs(node, adjList, used, map);
            }
        }
    }

    for (size_t i = 3; i < map.size(); i++) {
        for (size_t j = 3; j < map[i].size(); j++) {
            if (map[i][j] == '.') {
                if (map[i - 2][j] == '.'
                    && map[i][j - 2] == '.'
                    && map[i - 2][j - 2] == '.') {
                    double p = (rand() % 100) / 100.0;
                    if (p <= prob) {
                        for (int i0 = -2; i0 < 1; i0++) {
                            for (int j0 = -2; j0 < 1; j0++) {
                                map[i + i0][j + j0] = '.';
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i0 = -2; i0 < 1; i0++) {
        for (int j0 = -2; j0 < 1; j0++) {
            map[map.size() / 2 + i0][map[map.size() / 2].size() / 2 + j0] = '.';
        }
    }
}

void cellAutomatonSimulate(util::GameInfo& game);