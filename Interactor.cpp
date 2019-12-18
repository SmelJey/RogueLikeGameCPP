#include "Interactor.hpp"

int Interactor::lastKey = 0;

bool Interactor::isKeyPressed(int keyCode) {
    //flushinp();
    //lastKey = wgetch(stdscr);
    return lastKey == keyCode;
}

void Interactor::updateKeyState() {
    //flushinp();
    lastKey = wgetch(stdscr);
    
}
