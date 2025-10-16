#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

struct Player{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed;

    Uint64 lastShootTime;
    Uint64 coolDown = 100;
};  

struct ProjectilePlayer{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed = 100;
};

struct Enemy{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed = 100;

    Uint64 lastShootTime;
    Uint64 coolDown = 1000;
};

struct ProjectileEnemy{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed = 100;
};
#endif