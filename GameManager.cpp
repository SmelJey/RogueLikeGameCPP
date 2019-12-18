#pragma once

#include "Utility.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "MeleeEnemy.hpp"
#include "RangeEnemy.hpp"
#include "Interactor.hpp"
#include "GameManager.hpp"

#include <curses.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <thread>

#define player game.entities[0].get()

using json = nlohmann::json;

void destrWin(WINDOW* win) {
    if (win != nullptr) {
        wclear(win);
        wrefresh(win);
        delwin(win);
    }
}

GameManager::GameManager() : mapWindow(nullptr, destrWin), infoWindow(nullptr, destrWin) {};

void GameManager::randomSpawn() {
    for (auto& spec : meleeSpecs) {
        if (game.entities.size() > maxEntitiesCount)
            return;
        double p = (rand() % 100) / 100.0;
        double difficultyLvl = (curLevel < 20 ? curLevel * 1.0 / 100 : 0.2 + 0.5 - 1.0 / (curLevel - 18));
        if (p < spec.spawnRate + difficultyLvl) {
            util::Point checkPoint(rand() % mapWidth, rand() % mapHeight);
            if (util::checkPoint(game.map, checkPoint) && game[checkPoint] == '.' && util::distance(checkPoint, player->getPos()) > playerSight + 5) {
                spec.instantiate(game, checkPoint);
            }
        }
    }

    for (auto& spec : rangeSpecs) {
        if (game.entities.size() > maxEntitiesCount)
            return;
        double p = (rand() % 100) / 100.0;
        if (p < spec.spawnRate) {
            util::Point checkPoint(rand() % mapWidth, rand() % mapHeight);
            if (util::checkPoint(game.map, checkPoint) && game[checkPoint] == '.' && util::distance(checkPoint, player->getPos()) > playerSight + 5) {
                spec.instantiate(game, checkPoint);
            }
        }
    }
}

void GameManager::run() {
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1200, 800, TRUE);
    initscr();
    noecho();
    halfdelay(1);
    curs_set(FALSE);

    showMenu("Start your journey");

    init();
    if (seed == 0)
        seed = clock();

    while (true) {
        srand(seed);
        while (true) {
            curLevel++;
            if (!runLevel()) {
                break;
            }
        }
        gameOver();
    }
    
}

void GameManager::showMenu(const std::string& message) {
    auto menuWindow = win_ptr(newwin(menuHeight, menuWidth, 0, 0), destrWin);
    while (true) {
        Interactor::updateKeyState();
        wclear(menuWindow.get());
        if (Interactor::isKeyPressed(27)) {
            exit(0);
        }
        if (Interactor::isKeyPressed(' ')) {
            break;
        }
        box(menuWindow.get(), 0, 0);

        mvwprintw(menuWindow.get(), menuHeight / 2 - 10, menuWidth / 2 - 13, "auto game_name = \"test game\"");
        mvwprintw(menuWindow.get(), menuHeight / 2 - 3, menuWidth / 2 - message.length() / 2, message.c_str());
        mvwprintw(menuWindow.get(), menuHeight / 2 + 1, menuWidth / 2 - 10, "Press SPACE to start");
        mvwprintw(menuWindow.get(), menuHeight / 2 + 3, menuWidth / 2 - 9, "Press ESC to exit");

        mvwprintw(menuWindow.get(), menuHeight / 2 + 10, menuWidth / 2 - errMessage.length() / 2, errMessage.c_str());
        wrefresh(menuWindow.get());
    }
}

void GameManager::rewriteSettings() {
    json settings;

    settings["map"]["width"] = mapWidth;
    settings["map"]["height"] = mapHeight;
    settings["player"]["hp"] = 10;
    settings["player"]["dmg"] = 3;
    settings["player"]["shotDmg"] = 3;
    settings["player"]["sightRange"] = 20;

    settings["meleeEnemies"]["Z"]["maxHp"] = 10;
    settings["meleeEnemies"]["Z"]["moveCd"] = 4;
    settings["meleeEnemies"]["Z"]["dmg"] = 1;
    settings["meleeEnemies"]["Z"]["sightRange"] = 15;
    settings["meleeEnemies"]["Z"]["spawnRate"] = 0.2;

    settings["rangeEnemies"]["S"]["maxHp"] = 10;
    settings["rangeEnemies"]["S"]["moveCd"] = 3;
    settings["rangeEnemies"]["S"]["dmg"] = 1;
    settings["rangeEnemies"]["S"]["sightRange"] = 15;
    settings["rangeEnemies"]["S"]["shotCd"] = 10;
    settings["rangeEnemies"]["S"]["shotDmg"] = 3;
    settings["rangeEnemies"]["S"]["spawnRate"] = 0.1;

    settings["items"]["+"]["healRestoration"] = 3;
    settings["items"]["+"]["spawnRate"] = 0.0025;

    settings["items"]["%"]["ammo"] = 4;
    settings["items"]["%"]["spawnRate"] = 0.005;

    std::ofstream settingsOut("settings.json");
    settingsOut << settings.dump(2);
    settingsOut.close();
}

inline bool GameManager::isFileExist(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good() && f.peek() != std::ifstream::traits_type::eof();
}

void GameManager::init() {
    if (!isFileExist("settings.json")) {
        rewriteSettings();
    }
    std::ifstream settingsIn("settings.json");
    json settings;
    settingsIn >> settings;
    settingsIn.close();

    try {
        mapWidth = settings["map"]["width"];
        mapHeight = settings["map"]["height"];
        playerHp = settings["player"]["hp"];
        playerDmg = settings["player"]["dmg"];
        playerShotDmg = settings["player"]["shotDmg"];
        playerSight = settings["player"]["sightRange"];

        game.itemsProps["HealPotionRestoration"] = settings["items"]["+"]["healRestoration"];
        game.itemsProps["HealPotionSpawnRate"] = settings["items"]["+"]["spawnRate"];
        game.itemsProps["AmmoRestoration"] = settings["items"]["%"]["ammo"];
        game.itemsProps["AmmoSpawnRate"] = settings["items"]["%"]["spawnRate"];

    } catch (const json::exception&) {
        errMessage = "Error while handling settings.json";
    }
    if (settings["map"].contains("seed")) {
        seed = settings["map"]["seed"];
    }

    game.entities.push_back(std::unique_ptr<Player>(new Player(playerHp, playerDmg, playerShotDmg, util::Point(mapWidth / 2, mapHeight / 2))));

    if (errMessage.empty()) {
        try {
            for (auto& [chr, params] : settings["meleeEnemies"].items()) {
                MeleeEnemy me(chr[0], params["maxHp"].get<int>(),
                    params["moveCd"].get<int>(), params["dmg"].get<int>(),
                    params["sightRange"].get<int>(), *player);
                meleeSpecs.push_back(EntitySpec<MeleeEnemy>(me, params["spawnRate"].get<double>()));
            }

            for (auto& [chr, params] : settings["rangeEnemies"].items()) {
                RangeEnemy me(chr[0], params["maxHp"].get<int>(),
                    params["moveCd"].get<int>(), params["dmg"].get<int>(),
                    params["sightRange"].get<int>(), *player,
                    params["shotCd"].get<int>(), params["shotDmg"].get<int>());
                rangeSpecs.push_back(EntitySpec<RangeEnemy>(me, params["spawnRate"].get<double>()));
            }
        } catch (const json::exception&) {
            errMessage = "Error while handling settings.json";
        }

    }
}

void GameManager::levelInit() {
    if (game.entities.size() > 0) {
        game.entities.resize(1);
        game.entities[0]->setPos(util::Point(1, 1));
        game.entities[0]->setEnabled();
    } else {
        game.entities.push_back(std::unique_ptr<Player>(new Player(playerHp, playerDmg, playerShotDmg, util::Point(1, 1))));
    }

    game.isWin = false;
    game.map.clear();
    game.items.clear();
    game.projectiles.clear();
    defaultMap.clear();

    defaultMap.resize(mapHeight);
    for (size_t i = 0; i < mapHeight; i++) {
        for (size_t j = 0; j < mapWidth; j++) {
            defaultMap[i].push_back('.');
        }
    }
    mapGenerator.generateMap(defaultMap, game);
}

void GameManager::gameOver() {
    mapWindow.reset();
    infoWindow.reset();
    game.entities[0]->setHp(playerHp);

    curLevel = 0;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(200ms);

    showMenu("You died");
}

bool GameManager::runLevel() {
    levelInit();

    mapWindow = win_ptr(newwin(mapHeight + 2, mapWidth + 2, 0, 0), destrWin);
    infoWindow = win_ptr(newwin(mapHeight + 2, 40, 0, mapWidth + 2), destrWin);

    while (true) {
        Interactor::updateKeyState();
        if (Interactor::isKeyPressed(27) || player->getHp() <= 0) {
            return false;
        }

        if (!errMessage.empty()) {
            showMenu(errMessage);
            continue;
        }
        randomSpawn();

        drawStats(infoWindow);
        box(infoWindow.get(), 0, 0);

        game.map = defaultMap;
        for (auto& entity : game.entities) {
            if (entity->isEnabled())
                entity->draw(game);
        }

        for (auto& item : game.items) {
            if (item->isEnabled())
                item->draw(game);
        }

        game.updateAll();

        if (game.isWin) {
            return true;
        }

        wclear(mapWindow.get());
        drawMap(mapWindow);
        box(mapWindow.get(), 0, 0);
        
        wrefresh(mapWindow.get());

        wrefresh(infoWindow.get());
        wclear(infoWindow.get());
        
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(120ms);
    }
}

void GameManager::drawMap(win_ptr& win) const {
    for (size_t i = 0; i < game.map.size(); i++) {
        for (size_t j = 0; j < game.map[i].size(); j++) {
            if (util::distance(player->getPos(), util::Point(j, i)) > playerSight)
                mvwaddch(win.get(), 1 + i, 1 + j, '~');
            else
                mvwaddch(win.get(), 1 + i, 1 + j, game.map[i][j]);
        }
    }
}

void GameManager::drawStats(win_ptr& win) const {
    mvwprintw(win.get(), 1, 1, "Level: %d", curLevel);
    mvwprintw(win.get(), 5, 1, "Player HP: ");
    for (int i = 0; i < player->getHp(); i++)
        mvwprintw(win.get(), 5, 13 + i * 2, "o");
    mvwprintw(win.get(), 7, 1, "Player shots: %d", dynamic_cast<Player*>(player)->getShots());
}
