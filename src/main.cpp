#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "Game.h"

int main(int, char**) {
    Game::getInstance().init();
    Game::getInstance().run();
    return 0;
}