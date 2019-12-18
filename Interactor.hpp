#pragma once

#include <curses.h>

class Interactor {
public:
    static bool isKeyPressed(int keyCode);

    static void updateKeyState();

protected:
    static int lastKey;
};