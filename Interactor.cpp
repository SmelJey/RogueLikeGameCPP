#include "Interactor.hpp"
#ifdef _WIN32
#include <Windows.h>
#endif

int Interactor::lastKey = 0;

bool Interactor::isKeyPressed(int keyCode) {
#ifdef _WIN32
    return (GetKeyState(VkKeyScan(keyCode)) & 0x8000);
#endif
    return lastKey == keyCode;
}

void Interactor::updateKeyState() {
#ifdef _WIN32
    return;
#endif
    lastKey = wgetch(stdscr);
    
    
}
