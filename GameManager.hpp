#pragma once

#include "Utility.hpp"

#include "CollisionManager.hpp"

#include <curses.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>
#include <Windows.h>

using json = nlohmann::json;

class GameManager {
public:
    GameManager();

    void run();

private:
    inline bool isFileExist(const std::string& name);

    void rewriteSettings();

    void init();

    void drawMap(std::unique_ptr<WINDOW>& win);

    // settings.
    size_t mapWidth = 80;
    size_t mapHeight = 40;
    int playerHp = 10;
    int playerDmg = 10;
    std::unique_ptr<WINDOW> mapWindow;

    util::GameInfo game;

    std::vector<std::string> defaultMap;
    CollisionManager colManager;
};