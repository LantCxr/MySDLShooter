#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

struct Player{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed;
    int hp = 3;

    Uint64 lastShootTime;
    Uint64 coolDown = 100;
};  

struct ProjectilePlayer{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed = 100;

    int damage = 1;//玩家子弹伤害
};

struct Enemy{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed = 100;
    int hp = 2;

    Uint64 lastShootTime;
    Uint64 coolDown = 1000;
};

struct ProjectileEnemy{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_FPoint direction{0, 0};
    SDL_Texture* texture = nullptr;
    float speed = 0.005;

    int damage = 1;//敌机子弹伤害
};
#endif