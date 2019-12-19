#pragma once

#include "Utility.hpp"
#include "MeleeEnemy.hpp"
#include "RangeEnemy.hpp"

#include "MapGenerator.hpp"

#include <curses.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>
#include <Windows.h>

using json = nlohmann::json;

void destrWin(WINDOW* win);

using win_ptr = std::unique_ptr<WINDOW, std::function<void(WINDOW*)>>;

class GameManager {
public:
    template<typename T>
    struct EntitySpec {
        EntitySpec(T inst, double spawnRate) : instance(inst), spawnRate(spawnRate) {}

        void instantiate(util::GameInfo& game, util::Point pos) const {
            game.entities.push_back(std::unique_ptr<Entity>(new T(game.getNextId(), pos, instance)));
        }

        T instance;
        double spawnRate;
    };

    GameManager();

    void run();

private:
    void showMenu(const std::string& message);

    void rewriteSettings();

    inline bool isFileExist(const std::string& name);

    void init();

    void levelInit();

    void gameOver();

    bool runLevel();

    void drawMap(win_ptr& win) const;

    void drawStats(win_ptr& win) const;

    void randomSpawn();

    // settings.
    size_t mapWidth = 80;
    size_t mapHeight = 40;
    int playerHp = 10;
    int playerDmg = 3;
    int playerShotDmg = 2;
    int playerSight = 20;

    size_t maxEntitiesCount = 150;
    int curLevel = 0;
    int seed = 0;

    win_ptr mapWindow;
    win_ptr infoWindow;
    
    std::string errMessage = "";

    util::GameInfo game;

    std::vector<std::string> defaultMap;

    std::vector<EntitySpec<MeleeEnemy>> meleeSpecs;
    std::vector<EntitySpec<RangeEnemy>> rangeSpecs;

    MapGenerator mapGenerator;

    const size_t menuHeight = 40;
    const size_t menuWidth = 120;
    const size_t infoWindowWidth = 40;

    const size_t minMapWidth = 10;
    const size_t minMapHeight = 10;
    const size_t maxMapWidth = 105;
    const size_t maxMapHeight = 45;
};