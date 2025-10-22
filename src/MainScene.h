#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "SceneBase.h"
#include "Object.h"
#include <list>
#include <random>
#include <map>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

class Game;

class MainScene: public SceneBase
{
public:
    MainScene();
    ~MainScene();

    virtual void init() override;
    virtual void update(double deltaTime) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event* event) override;

    void keyboardControl(double deltaTime);

    void shootPlayer();
    void updateProjectiles(double deltaTime);
    void renderProjectiles();

    void spawnEnemy();
    void updateEnemies(double deltaTime);
    void renderEnemies();

    void spawnProjectileEnemy(Enemy* enemy);
    void updateProjectileEnemies(double deltaTime);
    void renderProjectileEnemies();
    void enemyExplode(Enemy *enemy);
    void playerExplode();
    void updatePlayer(float deltaTime);

    void updateExplosions(double deltaTime);
    void renderExplosions();

    void dropItem(Enemy *enemy); //敌人死亡后掉落物品
    void updateItems(double deltaTime); //更新物品
    void renderItems(); //渲染物品
    void playerGetItem(Item *item); //玩家捡起物品

    void renderUI();
private:

    std::list<Enemy*> enemyList;
    Enemy enemyTemplate;

    Explosion explosionTemplate;
    std::list<Explosion*> explosionList; //存储活动爆炸动画的列表

    std::list<ProjectileEnemy*> projectileEnemyList;
    ProjectileEnemy projectileEnemyTemplate;

    std::mt19937 gen; //随机数生成器
    std::uniform_real_distribution<float> dis;//随机数分布器
    Player player;
    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayerList;
    Game &game;

    bool isDead = false;

    Item itemLifeTemplate; //生命物品模板
    std::list<Item*> itemList; //存储活动物品的列表

    Mix_Music* bgm;
    std::map<std::string, Mix_Chunk*> sounds; //存储声音的map

    SDL_Texture* uiHealth;

    TTF_Font* scoreFont;
    int score = 0; //玩家得分
};

#endif