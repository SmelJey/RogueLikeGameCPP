#pragma once

#include "Utility.hpp"

#include "CollisionManager.hpp"
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
    GameManager();

    void run();

private:
    void showMenu();

    void gameOver();

    bool runLevel();

    inline bool isFileExist(const std::string& name);

    void rewriteSettings();

    void levelInit();

    void init();

    void drawMap(win_ptr& win);

    void drawStats(win_ptr& win);

    void randomSpawn();

    // settings.
    size_t mapWidth = 80;
    size_t mapHeight = 40;
    int playerHp = 10;
    int playerDmg = 3;
    int playerShotDmg = 2;
    
    int playerSight = 16; 
    int maxEntitiesCount = 150;
    int curLevel = 0;
    int seed = 0;

    win_ptr mapWindow;
    win_ptr infoWindow;

    util::GameInfo game;

    std::vector<std::string> defaultMap;
    CollisionManager colManager;
    MapGenerator mapGenerator;
};