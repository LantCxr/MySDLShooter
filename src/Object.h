#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

struct Player{
    float width, height;
    SDL_FPoint pos;
    SDL_Texture* texture;
    float speed;
};

#endif