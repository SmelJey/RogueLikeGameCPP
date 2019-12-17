#pragma once

#include "Utility.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "MeleeEnemy.hpp"
#include "RangeEnemy.hpp"
#include "GameManager.hpp"

#include <curses.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <thread>
#include <Windows.h>

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
        if (p < spec.spawnRate) {
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
    clock_t timeStart = clock();
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);

    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1200, 800, TRUE);
    initscr();
    noecho();
    curs_set(FALSE);

    showMenu();

    init();

    while (true) {
        for (int i = 0; i < 10; i++) {
            curLevel++;
            if (!runLevel()) {
                break;
            }
        }
        gameOver();
    }
    
}

void GameManager::showMenu() {
    auto menuWindow = win_ptr(newwin(40, 120, 0, 0), destrWin);
    while (true) {
        wclear(menuWindow.get());
        if (GetKeyState(VK_ESCAPE) & 0x8000) {
            exit(0);
        }
        if (GetKeyState(VK_SPACE) & 0x8000) {
            break;
        }

        mvwprintw(menuWindow.get(), 15, 47, "auto game_name = \"test game\"");
        mvwprintw(menuWindow.get(), 20, 50, "Press SPACE to start");
        mvwprintw(menuWindow.get(), 23, 51, "Press ESC to exit");
        wrefresh(menuWindow.get());
    }
}

void GameManager::gameOver() {
    mapWindow.reset();
    infoWindow.reset();

    auto menuWindow = win_ptr(newwin(40, 120, 0, 0), destrWin);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(200ms);
    while (true) {
        wclear(menuWindow.get());

        mvwprintw(menuWindow.get(), 15, 56, "You died");
        mvwprintw(menuWindow.get(), 20, 50, "Press SPACE to restart");
        mvwprintw(menuWindow.get(), 23, 51, "Press ESC to exit");
        wrefresh(menuWindow.get());

        if (GetKeyState(VK_ESCAPE) & 0x8000) {
            exit(0);
        }
        if (GetKeyState(VK_SPACE) & 0x8000) {
            break;
        }
    }
}

bool GameManager::runLevel() {
    levelInit();

    mapWindow = win_ptr(newwin(mapHeight + 2, mapWidth + 2, 0, 0), destrWin);
    infoWindow = win_ptr(newwin(mapHeight + 2, 40, 0, mapWidth + 2), destrWin);

    while (true) {
        randomSpawn();

        drawStats(infoWindow);
        box(infoWindow.get(), 0, 0);


        if (GetKeyState(VK_ESCAPE) & 0x8000 || player->getHp() <= 0) {
            return false;
        }

        game.map = defaultMap;
        for (auto& entity : game.entities) {
            if (entity->isEnabled())
                entity->draw(game);
        }

        for (auto& item : game.items) {
            if (item->isEnabled())
                item->draw(game);
        }

        for (int i = 0; i < game.projectiles.size(); i++) {
            if (game.projectiles[i]->isEnabled()) {
                game.projectiles[i]->update(this->game);
            }
        }

        for (int i = 0; i < game.projectiles.size(); i++) {
            if (game.projectiles[i]->isEnabled())
                break;
            game.projectiles.pop_front();
            i--;
        }

        for (int i = game.entities.size() - 1; i >= 0; i--) {
            if (game.entities[i]->isEnabled()) {
                game.entities[i]->update(this->game);
            }
        }

        if (game.isWin) {
            game.isWin = false;
            return true;
        }

        drawMap(mapWindow);
        box(mapWindow.get(), 0, 0);
        wrefresh(mapWindow.get());
        wclear(mapWindow.get());

        wrefresh(infoWindow.get());
        wclear(infoWindow.get());
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(120ms);
    }
}

inline bool GameManager::isFileExist(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good() && f.peek() != std::ifstream::traits_type::eof();
}

void GameManager::rewriteSettings() {
    json settings;

    settings["map"]["width"] = mapWidth;
    settings["map"]["height"] = mapHeight;
    settings["player"]["hp"] = playerHp;
    settings["player"]["dmg"] = playerDmg;
    settings["player"]["shotDmg"] = playerShotDmg;

    settings["meleeEnemies"]["Z"]["maxHp"] = 10;
    settings["meleeEnemies"]["Z"]["moveCd"] = 4;
    settings["meleeEnemies"]["Z"]["dmg"] = 1;
    settings["meleeEnemies"]["Z"]["sightRange"] = 15;
    settings["meleeEnemies"]["Z"]["spawnRate"] = 0.2;

    settings["rangeEnemies"]["S"]["maxHp"] = 10;
    settings["rangeEnemies"]["S"]["moveCd"] = 3;
    settings["rangeEnemies"]["S"]["dmg"] = 1;
    settings["rangeEnemies"]["S"]["sightRange"] = 15;
    settings["rangeEnemies"]["S"]["shotCd"] = 15;
    settings["rangeEnemies"]["S"]["shotDmg"] = 15;
    settings["rangeEnemies"]["S"]["spawnRate"] = 0.1;

    settings["items"]["h"]["healRestoration"] = 5;
    settings["items"]["h"]["spawnRate"] = 0.01;

    std::ofstream settingsOut("settings.json");
    settingsOut << settings.dump(2);
    settingsOut.close();
}

void GameManager::levelInit() {
    if (game.entities.size() > 0) {
        game.entities.resize(1);
        game.entities[0]->setPos(util::Point(1, 1));
        game.entities[0]->setHp(playerHp);
        game.entities[0]->setEnabled();
    } else {
        game.entities.push_back(std::unique_ptr<Player>(new Player(playerHp, playerDmg, playerShotDmg, util::Point(1,1))));
    }

    game.map.clear();
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

void GameManager::init() {
    if (!isFileExist("settings.json")) {
        rewriteSettings();
    }
    std::ifstream settingsIn("settings.json");
    json settings;
    settingsIn >> settings;
    settingsIn.close();

    mapWidth = settings["map"]["width"];
    mapHeight = settings["map"]["height"];
    playerHp = settings["player"]["hp"];
    playerDmg = settings["player"]["dmg"];
    playerShotDmg = settings["player"]["shotDmg"];

    game.itemsProps["HealPotionRestoration"] = settings["items"]["h"]["healRestoration"];
    game.itemsProps["HealPotionSpawnRate"] = settings["items"]["h"]["spawnRate"];

    game.entities.push_back(std::unique_ptr<Player>(new Player(playerHp, playerDmg, playerShotDmg, util::Point(mapWidth / 2, mapHeight / 2))));

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
}

void GameManager::drawMap(win_ptr& win) {
    for (size_t i = 0; i < game.map.size(); i++) {
        for (size_t j = 0; j < game.map[i].size(); j++) {
            if (util::distance(player->getPos(), util::Point(j, i)) > playerSight)
                mvwaddch(win.get(), 1 + i, 1 + j, '~');
            else
                mvwaddch(win.get(), 1 + i, 1 + j, game.map[i][j]);
        }
        //mvwprintw(win.get(), 1 + i, 1, game.map[i].c_str());
    }
}

void GameManager::drawStats(win_ptr& win) {
    mvwprintw(win.get(), 1, 1, "Level: %d", curLevel);
    mvwprintw(win.get(), 5, 1, "Player HP: %d", player->getHp());
}
