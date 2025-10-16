#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "SceneBase.h"
#include "Object.h"
#include <list>
#include <random>

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

private:

    std::list<Enemy*> enemyList;
    Enemy enemyTemplate;

    std::list<ProjectileEnemy*> projectileEnemyList;
    ProjectileEnemy projectileEnemyTemplate;

    std::mt19937 gen; //随机数生成器
    std::uniform_real_distribution<float> dis;//随机数分布器
    Player player;
    ProjectilePlayer projectilePlayerTemplate;
    std::list<ProjectilePlayer*> projectilePlayerList;
    Game &game;

};

#endif