#pragma once

#include "Utility.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Zombie.hpp"
#include "CollisionManager.hpp"
#include "GameManager.hpp"

#include <curses.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>
#include <Windows.h>

using json = nlohmann::json;

GameManager::GameManager() {};

void GameManager::run() {
    clock_t timeStart = clock();
    init();
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1200, 800, TRUE);
    initscr();
    noecho();
    curs_set(FALSE);

    Player player = Player(util::Point(mapWidth / 2, mapHeight / 2));
    player.setMaxHp(playerHp);
    player.setDmg(playerDmg);
    game.entities.push_back(std::unique_ptr<Entity>(new Zombie(util::Point(0, 0), dynamic_cast<Entity&>(player))));

    mapWindow = std::unique_ptr<WINDOW>(newwin(mapHeight + 2, mapWidth + 2, 0, 0));

    while (true) {
        if ((double)(clock() - timeStart) / CLOCKS_PER_SEC < 0.1)
            continue;
        timeStart = clock();
        
        game.map = defaultMap;
        player.draw(game);
        for (auto& entity : game.entities) {
            entity->draw(game);
        }


        for (auto& entity : game.entities) {
            colManager.addCollision(entity->update(this->game));
        }
        colManager.addCollision(player.update(game));
        colManager.process(game);

        drawMap(mapWindow);
        box(mapWindow.get(), 0, 0);
        wrefresh(mapWindow.get());
        wclear(mapWindow.get());
    }
}

inline bool GameManager::isFileExist(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good() && f.peek() != std::ifstream::traits_type::eof();
}

void GameManager::rewriteSettings() {
    json settings;

    settings["mapWidth"] = mapWidth;
    settings["mapHeight"] = mapHeight;
    settings["playerHp"] = playerHp;
    settings["playerDmg"] = playerDmg;

    std::ofstream settingsOut("settings.json");
    settingsOut << settings.dump(-1);
    settingsOut.close();
}

void GameManager::init() {
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
        //defaultMap[i].push_back('\n');
    }
}

void GameManager::drawMap(std::unique_ptr<WINDOW>& win) {
    for (size_t i = 0; i < game.map.size(); i++) {
        mvwprintw(win.get(), 1 + i, 1, game.map[i].c_str());
    }
}
