#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

struct Player{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    float speed;
    int hp = 3;
    int maxHealth = 3;

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

//爆炸结构
struct Explosion{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_Texture* texture = nullptr;
    int currentFrame = 0;
    int totalFrames = 8;
    Uint32 startTime = 0;
    Uint32 FPS = 10;
};

//物品枚举
enum class ItemType{
    Health,
    Shield,
    Time
};

//物品结构体
struct Item{
    float width, height;
    SDL_FPoint pos = {0,0};
    SDL_FPoint direction{0, 0};
    SDL_Texture* texture = nullptr;
    ItemType type = ItemType::Health;
    int duration = 0;
    int speed = 50;//物品下落速度
    int bounceCount = 3;//物品碰到屏幕边缘时反弹次数
};
#endif