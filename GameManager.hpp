#pragma once

#include "Utility.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Zombie.hpp"

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
    GameManager() {};

    void run() {
        clock_t timeStart = clock();
        init();
        HWND console = GetConsoleWindow();
        RECT ConsoleRect;
        GetWindowRect(console, &ConsoleRect);

        MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1200, 800, TRUE);
        initscr();

        Player player = Player(Point(mapWidth / 2, mapHeight / 2));
        player.setMaxHp(playerHp);
        player.setDmg(playerDmg);
        entities.push_back(std::unique_ptr<Entity>(new Zombie(Point(0, 0), player)));
        
        while (true) {
            if ((double)(clock() - timeStart) / CLOCKS_PER_SEC < 0.1)
                continue;
            timeStart = clock();

            map = defaultMap;
            for (auto& entity : entities) {
                entity->update(this->map);
            }

            player.update(map);
            drawMap();
            refresh();
            clear();
        }
    }

private:
    inline bool isFileExist(const std::string& name) {
        std::ifstream f(name.c_str());
        return f.good() && f.peek() != std::ifstream::traits_type::eof();
    }

    void rewriteSettings() {
        json settings;

        settings["mapWidth"] = mapWidth;
        settings["mapHeight"] = mapHeight;
        settings["playerHp"] = playerHp;
        settings["playerDmg"] = playerDmg;

        std::ofstream settingsOut("settings.json");
        settingsOut << settings.dump(-1);
        settingsOut.close();
    }

    void init() {
        if (!isFileExist("settings.json")) {
            rewriteSettings();
        }
        std::ifstream settingsIn("settings.json");
        json settings;
        settingsIn >> settings;
        settingsIn.close();

        mapWidth = settings["mapWidth"];
        mapHeight = settings["mapHeight"];

        defaultMap.resize(mapHeight);

        for (size_t i = 0; i < mapHeight; i++) {
            for (size_t j = 0; j < mapWidth; j++) {
                defaultMap[i].push_back('.');
            }
            defaultMap[i].push_back('\n');
        }
    }

    void drawMap() {
        for (size_t i = 0; i < map.size(); i++) {
            printw(map[i].c_str());
        }
    }


    // settings.
    size_t mapWidth = 80;
    size_t mapHeight = 40;
    int playerHp = 10;
    int playerDmg = 10;

    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::string> map;
    std::vector<std::string> defaultMap;
};